#include <reg52.h>
#include "time.h"
#include "key.h"
#include "smg.h"
#include "beep.h"
#include "public.h"

/*******************************************************************************
* �� �� ��       : answer_time
* ��������         : ѡ�ֻش𵹼�ʱ�����ɹ���ͣ�ش���ʧ�ܿ���������
* ��    ��       : error_plear��һ�׶�ѡ��̨�ţ�time_sec����ʱʱ��
* ��    ��         : ��
*******************************************************************************/
u8 answer_time(u8 error_plear,u16 time_sec)
{
   u8 key = 0;
   u8 time_buf[8];
   if(error_plear == 0)//�����ѡ����ǰ����ֱ����������ʱ����
   {
      return 0;
   }
   time0_start(time_sec);//��ʼ��ʱ
   while (1)
   {
      key = key_matrix_ranks_scan();//��ȡ���󰴼����µİ���
      if(key == ENTER)//�ش���ȷ�����ֹͣ��ʱ
      {
         time0_stop();//ֹͣ��ʱ
         while (1)
         {
            key = key_matrix_ranks_scan();//��ȡ���󰴼����µİ���
            if(key == REST)//������а����������˳�����ɹ�ʣ���ʱչʾ
               {
                  return 0;
               }
            time_buf[0]=gsmg_code[error_plear];//ѡ�ֱ��
		      time_buf[1]=0x40;
		      time_buf[2]=0x40;
		      time_buf[3]=0x40;
		      time_buf[4]=gsmg_code[last_gsec/1000];
		      time_buf[5]=gsmg_code[(last_gsec%1000)/100];
		      time_buf[6]=gsmg_code[(last_gsec%100)/10];
		      time_buf[7]=gsmg_code[last_gsec%10];          //����ʾ
            smg_display(time_buf,1);//��ʾ	
         }
      }
      else if(gsec <= 0)//����ʱ����
      {
         time0_stop();
         while (1)
         {
            key = key_matrix_ranks_scan();//��ȡ���󰴼����µİ���
            if(key == REST)//������а����������˳�����ʱ����չʾ
            {
               return 0;
            }
            time_buf[0]=gsmg_code[error_plear];//ѡ�ֱ��
		      time_buf[1]=0x40;
		      time_buf[2]=0x40;
		      time_buf[3]=0x40;
		      time_buf[4]=gsmg_code[0];
		      time_buf[5]=gsmg_code[0];
		      time_buf[6]=gsmg_code[0];
		      time_buf[7]=gsmg_code[0]; 
            smg_display(time_buf,1);//��ʾ
            beep_alarm(10,10);//��������
         }
         return 0;
      }
      else//ˢ��ʣ��ʱ��
      {
         time_buf[0]=gsmg_code[error_plear];//ѡ�ֱ��
		   time_buf[1]=0x40;
		   time_buf[2]=0x40;
		   time_buf[3]=0x40;
		   time_buf[4]=gsmg_code[gsec/1000];
		   time_buf[5]=gsmg_code[(gsec%1000)/100];
		   time_buf[6]=gsmg_code[(gsec%100)/10];
		   time_buf[7]=gsmg_code[gsec%10]; 
         smg_display(time_buf,1);//��ʾ	  
      }
   }
}

/*******************************************************************************
* �� �� ��       : answer
* ��������         : ��ȡ����ɹ���ѡ��̨��
* ��    ��       : ready�׶ε�ѡ��̨��
* ��    ��         : ������ɹ�����ѡ��̨�ţ�����һ�׶���ѡ����ǰ���𷵻�0
*******************************************************************************/
u8 answer(u8 error_plear)
{
   u8 key = 0;
   if(error_plear != 0)//�����ѡ����ǰ����ֱ�������������
   {
      return 0;
   }
   while (1)
   {
      key = key_scan(0);
      if(key != 0)//��ѡ������ɹ�����ѡ�ֱ��
      {
         return key;
      }
   }
   
}

/*******************************************************************************
* �� �� ��       : ready
* ��������         : �ж��Ƿ���ѡ����ǰ����
* ��    ��       : ��
* ��    ��         : �����ѡ�����𷵻�ѡ��̨�ţ����޷���0
*******************************************************************************/
u8 ready(void)
{
   u8 key = 0;
   u8 key_plear = 0;
   u8 ready_buf[8];
   ready_buf[0] = R;
   ready_buf[1] = E;
   ready_buf[2] = A;
   ready_buf[3] = D;
   ready_buf[4] = Y;
   ready_buf[5] = 0x40;
   ready_buf[6] = 0x40;
   ready_buf[7] = 0x40;
   while (1)
   {
      key = key_matrix_ranks_scan();//��ȡ���󰴼����µİ���
      key_plear = key_scan(0);//����Ƿ�ѡ����ǰ����
      if (key == ENTER)//���а��¿�ʼ��
      {
         break;
      }
      else if(key_plear != 0)//��ѡ����ǰ����
      {
         while(1)
         {
            key = key_matrix_ranks_scan();//��ȡ���󰴼����µİ���
            if(key == REST)//������а����������˳���ǰ����ѡ��չʾ
            {
               break;
            }
            ready_buf[0] = gsmg_code[key_plear];
            ready_buf[1] = 0x40;
            ready_buf[2] = 0x40;
            ready_buf[3] = E;
            ready_buf[4] = R;
            ready_buf[5] = R;
            ready_buf[6] = O;
            ready_buf[7] = R;
            smg_display(ready_buf,1);//��ʾ��ǰ����ѡ�ֺʹ�����Ϣ
            beep_alarm(10,10);
         }
         return key_plear;
      }
      else//��ѡ�������Ҳ���δ����ʼ��
      {  
         smg_display(ready_buf,1);//��ʾ'READY'
      }
   }
   return 0;
}

/*******************************************************************************
* �� �� ��       : countdown_t
* ��������         : ��ȡ����ʱʱ��
* ��    ��       : ��
* ��    ��         : ����ʱʱ��
*******************************************************************************/
u16 countdown_t(void)
{
   u8 key = 0;
   u8 time_buf[8];//���浹��ʱ
   u8 min = 0;//����ʱ�ֶ���
   u16 sec = 0;//����ʱ�붨��
   while (1)
   {
      key = key_matrix_ranks_scan();//��ȡ���󰴼����µİ���
      if(key == MINUP && min < 99)//���ӵ���ʱ+1
      {
         min++;
         //beep_alarm(1000,10);//������ʾ��
      }
      else if(key == SECUP && sec < 99)//�뵹��ʱ+1
      {
         sec++;
         //beep_alarm(1000,10);//������ʾ��
      }
      else if(key == REST)//���õ���ʱ
      {
         min = 0;
         sec = 0;
         //beep_alarm(1000,10);//������ʾ��
      }
      else if(key == ENTER)//����ʱʱ��ȷ��
      {
         //beep_alarm(1000,10);//������ʾ��
         break;
      }
      time_buf[0] = gsmg_code[min/10];
      time_buf[1] = gsmg_code[min%10];//��
      time_buf[2] = 0x40;//-
		time_buf[3] = gsmg_code[sec/10];
		time_buf[4] = gsmg_code[sec%10];//��
		time_buf[5] = 0x40;//-
      time_buf[6] = 0x40;//-
      time_buf[7] = 0x40;//-
      smg_display(time_buf,1);//��ʾ	
   }
   sec = sec + min*60;
   return sec;
}

/*******************************************************************************
* �� �� ��       : main
* ��������         : ����������
* ��    ��       : ��
* ��    ��         : ��
*******************************************************************************/
void main()                                
{   
   u8 answer_plear;                 //����ѡ��
   u16 time_sec;                     //����ʱʱ��
   time0_init();
   while(1)
    {
      time_sec = 0;               //���õ���ʱʱ��
      answer_plear = 0;          //���óɹ������ѡ��
      time_sec = countdown_t();  //��ȡ����ʱʱ��
      answer_plear = ready();     //׼������
      answer_plear = answer(answer_plear);     //��������ɹ���ѡ��
      answer_time(answer_plear,time_sec); 
    }
}
