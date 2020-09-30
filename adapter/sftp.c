#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#define REMOTE_URL "sftp://127.0.0.1/sftpuser/sftp-test/"

#define STRING_SIZE 100
/* NOTE: if you want this example to work on Windows with libcurl as a
   DLL, you MUST also provide a read callback with CURLOPT_READFUNCTION.
   Failing to do so will give you a crash since a DLL may not use the
   variable's memory when passed in to it from an app like this. */
static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
    curl_off_t nread;
    /* in real-world cases, this would probably get this data differently
     as this fread() stuff is exactly what the library already would do
     by default internally */
    size_t retcode = fread(ptr, size, nmemb, stream);

    nread = (curl_off_t)retcode;

    fprintf(stderr, "*** We read %" CURL_FORMAT_CURL_OFF_T " bytes from file\n", nread);
    return retcode;
}

int *sftp_upload(void *asptr, void *fileptr)
{
    char *as = (char *)asptr; // should be unique inorder to identify
    char *file = (char *)fileptr;
    CURL *curl;
    CURLcode res;
    FILE *fp;
    struct stat file_info;
    curl_off_t fsize;

    struct curl_slist *headerlist = NULL;

    /* get the file size of the local file */
    if (stat(file, &file_info))
    {
        printf("Couldn't open '%s': %s\n", file, strerror(errno));
        return strdup("failed to open");
    }
    fsize = (curl_off_t)file_info.st_size;

    printf("Local file size: %" CURL_FORMAT_CURL_OFF_T " bytes.\n", fsize);

    /* get a FILE * of the same file */
    fp = fopen(file, "rb");

    curl_global_init(CURL_GLOBAL_ALL);

    /* get a curl handle */
    curl = curl_easy_init();
    if (curl)
    {
        /* build a list of commands to pass to libcurl */

        /* we want to use our own read function */
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);

        /* enable uploading */
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

        /* Create unique target based on type */
        char url[STRING_SIZE];
        strcpy(url, REMOTE_URL);
        strcat(url, as);
        /* specify target */
        curl_easy_setopt(curl, CURLOPT_URL, url);

        /* now specify which file to upload */
        curl_easy_setopt(curl, CURLOPT_READDATA, fp);

        /* Set the size of the file to upload   If you give a *_LARGE
       option you MUST make sure that the type of the passed-in argument is a
       curl_off_t. If you use CURLOPT_INFILESIZE (without _LARGE) you must
       make sure that to pass in a type 'long' argument. */
        curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,
                         (curl_off_t)fsize);

        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_USERPWD, "sftpuser:password");
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        /* Now run off and do what you've been told! */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
            return 0;
        }

        /* clean up the SFTP commands list */
        curl_slist_free_all(headerlist);

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    fclose(fp); /* close the local file */

    curl_global_cleanup();
    return 1;
}
