int process_esb_request(char* bmd_file_path);
//int insert_in_esb_request(BMD *);
int select_active_route(const unsigned char *Sender, const unsigned char *Destination, const unsigned char *MessageType);
int select_transport_config(int route_id);
int select_transform_config(int route_id);
