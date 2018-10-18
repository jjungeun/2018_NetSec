
#include "packet_controller.h"
#include "packet.h"
#include "codec.h"
#include <string.h>

int Packet_Handler(unsigned char *src, unsigned char **dst, int msgType, int *dst_len)
{
	LOGIN_ACK *loginAck;
	IMG_SEND *imgSend;
	IMG_ACK *imgAck;
	unsigned char *imgBuf;
	FILE *fp;
	int rv;
	int length;

	switch(msgType){
	case MT_LOGIN_ACK:
		printf("\nClient received LOGIN_ACK packet.\n");
		decode_LoginAck(src, &loginAck);
		printf("\nLogin Result Msg: %s\n", loginAck->res_msg);
		free(loginAck);
		
		// 로그인 성공 시 이미지파일 전송
		fp = fopen("image.jpg","rb");
		if(fp != NULL)
		{
			imgSend = (IMG_SEND *)calloc(1,sizeof(IMG_SEND));
			fseek(fp,0,SEEK_END);
			length = ftell(fp);
			imgSend->imgLength = length;   

			fseek(fp,0,SEEK_SET);
			imgSend->img = (unsigned char *)calloc(length,sizeof(unsigned char));
			fread(imgSend->img,length,1,fp);
			fclose(fp);
			(*dst_len) = encode_packet(MT_IMG_SEND,(void *)imgSend,dst);
		}
		rv = 0;
		break;
	case MT_IMG_ACK:
		printf("\nClient received IMG_ACK packet.\n");
		decode_ImgAck(src, &imgAck);
		printf("\nIMG Result Msg: %s\n",imgAck->res_msg);
		free(imgAck);
		rv=0;
		break;

	default:
		rv = -1;
		break;
	}
	return rv;
}
