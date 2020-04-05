#include "key.h"
#include "beep.h"




//-------------------------------

//�ṹ�嶨��
typedef struct
{
    uint8_t lock;//����������־/
    uint16_t timeCnt;//����ȥ������ʱ������//key_up.timeCnt
    uint8_t timeCntDelay;//��ס��������������������
    uint8_t reTimeCntThreshold;//��ס��������������������ֵ�������ȼ��ٴ�������
    uint8_t shortTouchFlag;//�����ڳ�����Ϊһ����ֵ
} KEY_STRUCT;

KEY_STRUCT key_up;
KEY_STRUCT key_ok;
KEY_STRUCT key_down;


//��������
uint8_t keyNum = 0;



//��������


void KEY_Scan(void)//����ɨ�躯�� ���ڶ�ʱ�ж���
{

    if(KEY_UP_IN==1)//IO�Ǹߵ�ƽ��˵������û�б����£�
    {
        key_up.lock=0; //����������־����
        key_up.timeCnt=0;//����ȥ������ʱ���������㣬

        if(key_up.shortTouchFlag==1)  //�̰�������־
        {
            key_up.shortTouchFlag=0;
            keyNum = 1;//�����̰���ֵ1
            BEEP_ON(30);
            printf("�ϼ�����\r\n");

        }
    }
    else if(key_up.lock==0)//�а������£����ǵ�һ�α�����
    {
        key_up.timeCnt++; //�ۼӶ�ʱ�жϴ���
        if(key_up.timeCnt>KEYSHORTTIME)
        {
            key_up.shortTouchFlag=1;   //������̰�����Ч��־
        }

        if(key_up.timeCnt>KEYLONGTIME)
        {
            key_up.shortTouchFlag=0;  //��������̰�����Ч��־

            key_up.timeCnt=0;
            key_up.lock=1;  //����������λ,����һֱ����

            keyNum=4;    //����1�ż��ĳ���

        }
    }



    if(KEY_OK_IN==1)//IO�Ǹߵ�ƽ��˵������û�б����£�
    {
        key_ok.lock=0; //����������־����
        key_ok.timeCnt=0;//����ȥ������ʱ���������㣬

        if(key_ok.shortTouchFlag==1)  //�̰�������־
        {
            key_ok.shortTouchFlag=0;
            keyNum = 2;//�����̰���ֵ1
            BEEP_ON(30);
            printf("�м�����\r\n");

        }
    }
    else if(key_ok.lock==0)//�а������£����ǵ�һ�α�����
    {
        key_ok.timeCnt++; //�ۼӶ�ʱ�жϴ���
        if(key_ok.timeCnt>KEYSHORTTIME)
        {
            key_ok.shortTouchFlag=1;   //������̰�����Ч��־
        }

        if(key_ok.timeCnt>KEYLONGTIME)
        {
            key_ok.shortTouchFlag=0;  //��������̰�����Ч��־

            key_ok.timeCnt=0;
            key_ok.lock=1;  //����������λ,����һֱ����

            keyNum=5;    //����1�ż��ĳ���
					BEEP_ON(60);
					printf("�м�������\r\n");
        }
    }


    if(KEY_DOWN_IN==1)//IO�Ǹߵ�ƽ��˵������û�б����£�
    {
        key_down.lock=0; //����������־����
        key_down.timeCnt=0;//����ȥ������ʱ���������㣬

        if(key_down.shortTouchFlag==1)  //�̰�������־
        {
            key_down.shortTouchFlag=0;
            keyNum = 3;//�����̰���ֵ1
            BEEP_ON(30);
            printf("�¼�����\r\n");

        }
    }
    else if(key_down.lock==0)//�а������£����ǵ�һ�α�����
    {
        key_down.timeCnt++; //�ۼӶ�ʱ�жϴ���
        if(key_down.timeCnt>KEYSHORTTIME)
        {
            key_down.shortTouchFlag=1;   //������̰�����Ч��־
        }

        if(key_down.timeCnt>KEYLONGTIME)
        {
            key_down.shortTouchFlag=0;  //��������̰�����Ч��־

            key_down.timeCnt=0;
            key_down.lock=1;  //����������λ,����һֱ����

            keyNum=6;    //����1�ż��ĳ���
        }
    }




}





// ������ʼ��
//---------------------
void KEY_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;						// ����GPIO_InitTypeDef�ͽṹ��


    // ʹ��GPIOB�˿�ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	// ʹ��GPIOʱ��


    // ������UP DOWN OK
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;				//b12
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 			// ��������
    GPIO_Init(GPIOB, &GPIO_InitStructure);					// �����趨������ʼ��


    //�����ṹ���ʼ��
    key_up.lock = 0;
    key_up.timeCnt = 0;
    key_up.shortTouchFlag = 0;

    key_ok.lock = 0;
    key_ok.timeCnt = 0;
    key_ok.shortTouchFlag = 0;

    key_down.lock = 0;
    key_down.timeCnt = 0;
    key_down.shortTouchFlag = 0;
}



//------------------------------------------

