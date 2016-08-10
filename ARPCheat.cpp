#include <stdlib.h>
#include <stdio.h>

#include <pcap.h>


int main(int argc, char **argv)
{
    pcap_t *fp;
    char errbuf[PCAP_ERRBUF_SIZE];
    u_char packet[60];
    int j;
    pcap_if_t *alldevs;
    pcap_if_t *d;
    int inum;
    int i = 0;




    /* ��������в����ĺϷ��� */
    if (argc != 2)
    {
        printf("��ʽ�Ƿ�\n", argv[0]);
        return 0;
    }

    /* ��ȡ�����豸�б� */
    if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1)
    {
        fprintf(stderr,"Error in pcap_findalldevs: %s\n", errbuf);
        exit(1);
    }

    /* ��ӡ�б� */
    for(d=alldevs; d; d=d->next)
    {
        printf("%d. %s", ++i, d->name);
        if (d->description)
            printf(" (%s)\n ", d->description);
        else
            printf(" ���޿�������\n");
    }

    if(i==0)
    {
        printf("\nû�з����κ��豸�����Ȱ�װWinPcap\n");
        return -1;
    }

    printf("�����豸�ӿں� (1-%d):", i);
    scanf_s("%d", &inum);

    if(inum < 1 || inum > i)
    {
        printf("\n �Ƿ����豸�ӿں� \n");
        /* �ͷ��豸�б� */
        pcap_freealldevs(alldevs);//һ��Ҫ�ǵ��ͷ�
        return -1;
    }

    /* ��ת��ѡ�е������� */
    for(d=alldevs, i=0; i<inum-1 ;d=d->next, i++);

    /* ���豸 */
    if ( (fp= pcap_open(d->name,          // �豸��
                        65536,            // 65535��֤�ܲ��񵽲�ͬ������·���ϵ�ÿ�����ݰ���ȫ������
						PCAP_OPENFLAG_PROMISCUOUS,// ����ģʽ
                        1000,             // ��ȡ��ʱʱ��
                        NULL,             // Զ�̻�����֤
                        errbuf )          // ���󻺳��
                        ) == NULL)
    {
        fprintf(stderr,"\n�޷�����������WinPcap��֧���豸%s\n", d->name);
        /* �ͷ��豸�б� */
        pcap_freealldevs(alldevs);
        return -1;
    }
    
    printf("\n���� %s...\n", d->description);
    
    /* �ͷ��豸�б� */
    pcap_freealldevs(alldevs);

    /* �������ÿ��Ը��Ի�ȥ�޸ģ���������ʱ��ǵ�αװ���Լ������ⱻ���� */
    /* ����MAC��Ŀ�ĵ�ַΪ ff:ff:ff:ff:ff:ff */
    packet[0]=0xff;
    packet[1]=0xff;
    packet[2]=0xff;
    packet[3]=0xff;
    packet[4]=0xff;
    packet[5]=0xff;
    
    /* ����MACԴ��ַΪ f4:6d:04:f9:70:4e */
    packet[6]=0xf4;
    packet[7]=0x6d;
    packet[8]=0x04;
    packet[9]=0xf9;
    packet[10]=0x70;
    packet[11]=0x4e;
   //֡���� 
    packet[12]=0x08;
    packet[13]=0x06;
   //Ӳ������
    packet[14]=0x00;
    packet[15]=0x01;
	//Э������
    packet[16]=0x08;
    packet[17]=0x00;
	//Ӳ����ַ����
    packet[18]=0x06;
	//Э���ַ����
    packet[19]=0x04;
	//�����룬1��ʾ����
    packet[20]=0x00;
    packet[21]=0x01;
	//ԴMAC��ַme
    packet[22]=0xf4;
    packet[23]=0x6d;
    packet[24]=0x04;
    packet[25]=0xf9;
    packet[26]=0x70;
    packet[27]=0x4e;
	//αװ��ԴIP��ַ��115.155.39.156��
    packet[28]=0x73;
    packet[29]=0x9b;
    packet[30]=0x27;
    packet[31]=0x9c;
	//Ŀ��MAC��Ϊȫ0
    packet[32]=0x00;
    packet[33]=0x00;
    packet[34]=0x00;
    packet[35]=0x00;
    packet[36]=0x00;
    packet[37]=0x00;
	//Ŀ��IP��115.155.39.190��
    packet[38]=0x73;
    packet[39]=0x9b;
    packet[40]=0x27;
    packet[41]=0xbe;
    /* ���ʣ�µ����� */
    for(j=42;j<60;j++)
    {
        packet[j]=j%256;
    }

    /* �������ݰ� */
    if (pcap_sendpacket(fp, packet, 60 /* size */) != 0)
    {
        fprintf(stderr,"\n����ʧ�� \n", pcap_geterr(fp));
        return -1;
    }

    return 0;
}

