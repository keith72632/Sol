#ifndef MQTT_H
#define MQTT_H

#include <stdio.h>

//These are the fixed sizes of the MQTT header and Ack packets
#define MQTT_HEADER_LEN 2
#define MQTT_ACK_LEN 4

/*
* Stub bytes, useful for gernic replies, these represent the first byte in
*the mfixed header
*/

#define CONNACK_BYTE   0x20
#define PUBLISH_BYTE   0x30
#define PUBACK_BYTE    0x40
#define PUBREC_BYTE    0x50
#define PUBREL_BYTE    0x60
#define PUBCOMP_BYTE   0x70
#define SUBACK_BYTE    0x90
#define UNSUBACK_BYTE  0xB0
#define PINGRESP_BYTE  0xD0

/* Message types */
enum packet_type {
    CONNECT     = 1,
    CONNACK     = 2,
    PUBLISH     = 3,
    PUBACK      = 4,
    PUBREC      = 5,
    PUBREL      = 6,
    PUBCOMP     = 7,
    SUBSCRIBE   = 8,
    SUBACK      = 9,
    UNSUBSCRIBE = 10,
    UNSUBACK    = 11,
    PINGREQ     = 12,
    PINGRESP    = 13,
    DISCONNECT  = 14
};

enum qos_level {
	AT_MOST_ONCE,
	AT_LEAST_ONCE,
	EXACTLY_ONCE
};

/*control header is always a byte*/
union mqtt_header {
    unsigned char byte;
    struct {
        unsigned retain : 1;
        unsigned qos : 2;
        unsigned dup : 1;
        unsigned type : 4;
    } bits;
};

/*
Control Packet. First packet to be sent when a client established new connection
and must be exactly one. For each connect, a Connack mmust be sent in response
*/
struct mqtt_connect {
	union mqtt_header header;
	union {
		unsigned char byte;
		struct {
			int reserved              :1;
			unsigned clean_session    :1;
			unsigned will             :1;
			unsigned will_qos         :2;
			unsigned will_retain      :1;
			unsigned password         :1;
			unsigned username         :1;
		} bits;
	};
	struct {
		unsigned short keepalive;
		unsigned char *client_id;
		unsigned char *username;
		unsigned char *password;
		unsigned char *will_topic;
		unsigned char *will_message;
	} payload;
};

struct mqtt_connack{
	union mqtt_header header;
	union {
		unsigned char byte;
		struct {
			unsigned session_present   :1;
			unsigned reserved          :7;
		} bits;
	};
	unsigned char rc;
};

struct mqtt_subscribe {
	union mqtt_header header;
	unsigned short pkt_id;
	unsigned short tuple_len;
	struct {
		unsigned short topic_len;
		unsigned char *topic;
		unsigned qos;
	} * tuples;
};

struct mqtt_unsubscribe {
	union mqtt_header header;
	unsigned short pkt_id;
	unsigned short tuple_len;
	struct {
		unsigned short topic_len;
		unsigned char *topic;
	} * tuples;
};

struct mqtt_sub_ack {
	union mqtt_header header;
	unsigned short pkt_id;
	unsigned short rcs_len;
	unsigned char *rcs;
};

struct mqtt_publish {
	union mqtt_header header;
	unsigned short pkt_id;
	unsigned short topic_len;
	unsigned char *topic;
	unsigned short payload_len;
	unsigned char *payload;
};

struct mqtt_ack {
    union mqtt_header header;
    unsigned short pkt_id;
};

typedef struct mqtt_ack mqtt_puback;
typedef struct mqtt_ack mqtt_pubrec;
typedef struct mqtt_ack mqtt_pubrel;
typedef struct mqtt_ack mqtt_pubcomp;
typedef struct mqtt_ack mqtt_unsuback;
typedef union mqtt_header mqtt_pingreq;
typedef union mqtt_header mqtt_pingresp;
typedef union mqtt_header mqtt_disconnect;

union mqtt_packet {
	// This will cover PUBACK, PUBREC, PUBREL, PUBCOMP and UNSUBACK
	struct mqtt_ack ack;
    // This will cover PINGREQ, PINGRESP and DISCONNECT
	union mqtt_header header;
	struct mqtt_connect connect;
	struct mqtt_connack connack;
	struct mqtt_sub_ack suback;
	struct mqtt_publish publish;
	struct mqtt_subscribe subscribe;
	struct mqtt_unsubscribe unsubscribe;
};

/*
To handle the communication using the MQTT protocol we need essentially 4 functions, 2 for each direction of the interaction between server and client:

A packing function (serializing or marshalling, I won’t dive here in a dissertion on the correct usage of these terms)
An unpacking function (deserializing/unmarshalling)
Supported by 2 functions to handle the encoding and decoding of the Remaining Length in the Fixed Header part.
*/

int mqtt_encode_length(unsigned char *, size_t);
unsigned long long mqtt_decode_length(const unsigned char **);
int unpack_mqtt_packet(const unsigned char *, union mqtt_packet *);
unsigned char *pack_mqtt_packet(const union mqtt_packet *, unsigned);

//functions to build packets and release heap-alloced ones
union mqtt_header *mqtt_packet(unsigned char);
struct mqtt_ack *mqtt_packet_ack(unsigned char, unsigned short);
struct mqtt_connack *mqtt_packet_connack(unsigned char, unsigned char, unsigned char);
struct mqtt_sub_ack *mqtt_packet_suback(unsigned char, unsigned short, unsigned char *, unsigned short);
struct mqtt_publish *mqtt_packet_publish(unsigned char, unsigned short, size_t, unsigned char *, size_t, unsigned char *);
void mqtt_packet_release(union mqtt_packet *, unsigned);

#endif