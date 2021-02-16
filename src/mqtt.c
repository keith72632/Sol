#include <stdlib.h>
#include <string.h>
#include "mqtt.h"

/*
static size_t unpack_mqtt_connect(const unsigned char *str, union mqtt_header *header, union mqtt_packet *packet);
static size_t upack_mqtt_publish(const unsigned char *str, union mqtt_ header *header, union mqtt_packet *packet);
static size_t upack_mqtt_subscribe(const unsigned char *str, union mqtt_ header *header, union mqtt_packet *packet);
static size_t upack_mqtt_unsubscribe(const unsigned char *str, union mqtt_ header *header, union mqtt_packet *packet);
static size_t upack_mqtt_ack(const unsigned char *str, union mqtt_ header *header, union mqtt_packet *packet);
*/
static unsigned char *pack_mqtt_header(const union mqtt_header *header);
static unsigned char *pack_mqtt_ack(const union mqtt_packet *packet);
static unsigned char *pack_mqtt_connack(const union mqtt_packet *packet);
static unsigned char *pack_mqtt_suback(const union mqtt_packet *packet);
static unsigned char *pack_mqtt_publish(const union mqtt_packet *packet);

int main()
{
	printf("Compile successful\n");
	return 0;
}



