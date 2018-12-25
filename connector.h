#ifndef CONN_H
#define CONN_H

void createConnection();
void destroyConnection();
int haveConnection();
int read(void* data, size_t size);
int write(void* data, size_t size);

#endif
