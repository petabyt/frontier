char error_buffer[64];

char *errstr = NULL;

int sys_report_err(char *string) {
	errstr = string;
}

char *sys_get_error() {
	return errstr;
}
