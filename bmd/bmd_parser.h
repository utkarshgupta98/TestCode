#ifndef bmd_parser
#define bmd_parser
#define START_TAG 1 //value acoording to return type of xmlTextReaderNodeType
#define END_TAG 15

typedef struct {
    const unsigned char * key;
    void * value;
}
pair;

typedef struct {
    const unsigned char * MessageID;
    const unsigned char * MessageType;
    const unsigned char * Sender;
    const unsigned char * Destination;
    const unsigned char * CreationDateTime;
    const unsigned char * Signature;
    pair * UserProperties[50];
    const unsigned char * ReferenceID;

}
envelope;

typedef struct {
    const unsigned char * data;
}
payload;

typedef struct {
    envelope * bmd_envelope;
    payload * bmd_payload;
}
BMD;

BMD * parse_bmd_xml(char * );
BMD * process_xml(char * );
int check_if_string_is_guid(const unsigned char * value);
envelope * extract_envelop(char * bmd_xml);
payload * extract_payload(char * bmd_xml);

#endif /* bmd_parser.h */
