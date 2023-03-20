#pragma pack(push, 1)



struct AppMetaData {
	uint8_t magic1;
	uint8_t magic2;
	uint32_t api_version;
	uint32_t header_size;
	char name[20];
	char author[20];
	char url[40];
	uint8_t icon[];
};

#pragma pack(pop)
