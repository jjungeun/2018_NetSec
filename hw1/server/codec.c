#include "codec.h"
#include <string.h>

int encode_PacketHeader(PACKET_HEADER *src, unsigned char **dst)
{
	unsigned char *pt;
	int msgType, length;

	*dst = (unsigned char *)calloc(1, sizeof(PACKET_HEADER));
	pt = *dst;

	msgType = htonl(src->msgType);
	memcpy(pt, &msgType, sizeof(int));
	pt += sizeof(int);

	length = htonl(src->length);
	memcpy(pt, &length, sizeof(int));

	return sizeof(PACKET_HEADER);
}

int decode_PacketHeader(unsigned char *src, PACKET_HEADER **dst)
{
	unsigned char *pt;
	int msgType, length;

	pt = src;
	*dst = (PACKET_HEADER *)calloc(1, sizeof(PACKET_HEADER));

	memcpy(&msgType, pt, sizeof(int));
	(*dst)->msgType = ntohl(msgType);
	pt += sizeof(int);

	memcpy(&length, pt, sizeof(int));
	(*dst)->length = ntohl(length);

	return sizeof(PACKET_HEADER);
}

int encode_LoginReq(LOGIN_REQ *src, unsigned char **dst)
{
	unsigned char *pt;

	*dst = (unsigned char *)calloc(1, sizeof(LOGIN_REQ));
	pt = *dst;

	memcpy(pt, src->id, sizeof(src->id));
	pt += sizeof(src->id);

	memcpy(pt, src->passwd, sizeof(src->passwd));

	return sizeof(LOGIN_REQ);
}

int decode_LoginReq(unsigned char *src, LOGIN_REQ **dst)
{
	unsigned char *pt;

	pt = src;
	*dst = (LOGIN_REQ *)calloc(1, sizeof(LOGIN_REQ));

	memcpy((*dst)->id, pt, sizeof((*dst)->id));
	pt += sizeof((*dst)->id);

	memcpy((*dst)->passwd, pt, sizeof((*dst)->passwd));

	return sizeof(LOGIN_REQ);
}

int encode_LoginAck(LOGIN_ACK *src, unsigned char **dst)
{
	unsigned char *pt;
	int result;

	*dst = (unsigned char *)calloc(1, sizeof(LOGIN_ACK));
	pt = *dst;

	result = htonl(src->result);
	memcpy(pt, &result, sizeof(int));
	pt += sizeof(int);

	memcpy(pt, src->res_msg, sizeof(src->res_msg));

	return sizeof(LOGIN_ACK);
}

int decode_LoginAck(unsigned char *src, LOGIN_ACK **dst)
{
	unsigned char *pt;
	int result;

	pt = src;
	*dst = (LOGIN_ACK *)calloc(1, sizeof(LOGIN_ACK));

	memcpy(&result, pt, sizeof(int));
	(*dst)->result = ntohl(result);
	pt += sizeof(int);

	memcpy((*dst)->res_msg, pt, sizeof((*dst)->res_msg));

	return sizeof(LOGIN_ACK);
}

int encode_ImgSend(IMG_SEND *src, unsigned char **dst){
	unsigned char *pt;

	int len = htonl(src->imgLength);
	*dst = (unsigned char *)calloc(1, sizeof(int)+src->imgLength);
	pt = *dst;

	memcpy(pt,&len,sizeof(int));
	pt += sizeof(int);

	memcpy(pt, src->img, src->imgLength);

	return sizeof(int)+src->imgLength;
}

int decode_ImgSend(unsigned char *src, IMG_SEND **dst){
	unsigned char *pt;
	int len;

	pt = src;
	*dst = (IMG_SEND *)calloc(1, sizeof(IMG_SEND));

	memcpy(&len, pt, sizeof(int));
	(* dst)->imgLength = ntohl(len);
	pt += sizeof(int);

	(*dst)->img = (unsigned char *)calloc((*dst)->imgLength,sizeof(unsigned char));
	memcpy((*dst)->img, pt, (*dst)->imgLength);

	return sizeof(IMG_SEND);
}

int encode_ImgAck(IMG_ACK *src, unsigned char **dst){
	unsigned char *pt;
	int result;

	*dst = (unsigned char *)calloc(1, sizeof(IMG_ACK));
	pt = *dst;

	result = htonl(src->imgResult);
	memcpy(pt, &result, sizeof(int));
	pt += sizeof(int);

	memcpy(pt, src->res_msg, sizeof(src->res_msg));

	return sizeof(IMG_ACK);
}

int decode_ImgAck(unsigned char *src, IMG_ACK **dst){

	unsigned char *pt;
	int result;

	pt = src;
	*dst = (IMG_ACK *)calloc(1, sizeof(IMG_ACK));

	memcpy(&result, pt, sizeof(int));
	(*dst)->imgResult = ntohl(result);
	pt += sizeof(int);

	memcpy((*dst)->res_msg, pt, sizeof((*dst)->res_msg));

	return sizeof(IMG_ACK);
}

int encode_packet(int msgType, void *msg, unsigned char **dst)
{
	int msgLen, headLen;
	unsigned char *headBuf, *msgBuf, *pt;
	PACKET_HEADER *header;

	switch(msgType){
	case MT_LOGIN_REQ:
		msgLen = encode_LoginReq((LOGIN_REQ *)msg, &msgBuf); //msg encoding and get msg length
		header = (PACKET_HEADER *)calloc(1, sizeof(PACKET_HEADER)); //make header
		header->msgType = MT_LOGIN_REQ;
		header->length = msgLen;
		headLen = encode_PacketHeader(header, &headBuf); //encoding header and get header length
		break;
	case MT_LOGIN_ACK:
		msgLen = encode_LoginAck((LOGIN_ACK *)msg, &msgBuf); //msg encoding and get msg length
		header = (PACKET_HEADER *)calloc(1, sizeof(PACKET_HEADER)); //make header
		header->msgType = MT_LOGIN_ACK;
		header->length = msgLen;
		headLen = encode_PacketHeader(header, &headBuf); //encoding header and get header length
		break;
	case MT_IMG_SEND:
		msgLen = encode_ImgSend((IMG_SEND *)msg, &msgBuf);
		header = (PACKET_HEADER *)calloc(1,sizeof(PACKET_HEADER));
		header->msgType = MT_IMG_SEND;
		header->length = msgLen;
		headLen = encode_PacketHeader(header, &headBuf);
		break;
	case MT_IMG_ACK:
		msgLen = encode_ImgAck((IMG_ACK *)msg, &msgBuf);
		header = (PACKET_HEADER *)calloc(1,sizeof(PACKET_HEADER));
		header->msgType = MT_IMG_ACK;
		header->length = msgLen;
		headLen = encode_PacketHeader(header, &headBuf);
		break;
	}

	*dst = (unsigned char *)calloc(1, headLen + msgLen);
	pt = *dst;

	memcpy(pt, headBuf, headLen);
	pt += headLen;

	memcpy(pt, msgBuf, msgLen);

	free(headBuf);
	free(msgBuf);

	return (headLen + msgLen);
}


