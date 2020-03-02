#ifndef _VIDEO_CAPTURE_H
#define _VIDEO_CAPTURE_H


#include <string>
#include <memory>
#include <map>
#include "frame.h"
#include "dataType.h"
#include "videoComm.h"







/*
	��Ƶ�ɼ��������
*/
class AbsFrameCapture
{
	public:
		AbsFrameCapture(const std::string & capturePath);
		virtual ~AbsFrameCapture() = default;

		/*
		���ܣ�����Ƶ�豸

		������void

		����ֵ��true/false
		*/
		virtual bool open(void)=0;



		/*
		���ܣ��ر���Ƶ�豸

		������void

		����ֵ��true/false
		*/
		virtual bool close(void)=0;

		
		/*
			���ܣ���������ͷ�ɼ�֡��

			������<rate>	 ֡�ʣ�ÿ���Ӳ������Ƶ֡����

			����ֵ��true/false
		*/
		virtual bool setFrameRate(UINT16 rate) = 0;
		
		/*
			���ܣ�������Ƶ֡�ߴ�

			������<width>	  ͼ��Ŀ������ֵ
				  <height> ͼ��ĸ߶�����ֵ
				  
			����ֵ��true/false
		*/
		virtual bool setFrameSize(UINT16 width,UINT16 height)=0;



		/*
			���ܣ�������Ƶ֡��ʽ

			������<imgFormat>	��Ƶ֡�ĸ�ʽ				  
				  
			����ֵ��true/false
		*/
		virtual bool setFrameFmt(RawImgFmt_t imgFormat)=0;


		/*
			���ܣ��ӵײ��ȡһ֡ͼ��
			
			������void				  
				  
			����ֵ��Yuv֡��ָ��,���Ϊ�������ɼ�֡ʧ��
		*/
		virtual std::shared_ptr<YuvFrame> captureFrame(void) = 0;



		
	protected:

		//��ƵԴ����
		std::string m_capturePath;

		//ͼ��������ֵ
		UINT16 m_width;

		//ͼ��߶�����ֵ
		UINT16 m_height;

		//ͼ���ʽ
		RawImgFmt_t m_frameFmt;

};


class V4l2Capture:public AbsFrameCapture
{
	public:
		
	

	public:
		V4l2Capture(const std::string & capturePath);
		virtual ~V4l2Capture();

		/*
		���ܣ�����Ƶ�豸.����Ĺ��캯����Ĭ�ϵ���open����

		������void

		����ֵ��true/false
		*/
		virtual bool open(void) override;



		/*
		���ܣ��ر���Ƶ�豸

		������void

		����ֵ��true/false
		*/
		virtual bool close(void) override;


		
		/*
		���ܣ���������ͷ�ɼ�֡��

		������<rate>	 ֡�ʣ�ÿ���Ӳ������Ƶ֡����

		����ֵ��true/false
		*/
		virtual bool setFrameRate(UINT16 rate) override;
		
		/*
			���ܣ�������Ƶ֡�ߴ�

			������<width>	  ͼ��Ŀ������ֵ
				  <height> ͼ��ĸ߶�����ֵ
				  
			����ֵ��true/false
		*/
		virtual bool setFrameSize(UINT16 width,UINT16 height) override;



		
		/*
			���ܣ�������Ƶ֡��ʽ

			������<imgFormat>	��Ƶ֡�ĸ�ʽ				  
				  
			����ֵ��true/false
		*/
		virtual bool setFrameFmt(RawImgFmt_t imgFormat) override;


		/*
			���ܣ��ӵײ��ȡһ֡ͼ��
			
			������void				  
				  
			����ֵ��Yuv֡��ָ��,���Ϊ�������ɼ�֡ʧ��
		*/
		virtual std::shared_ptr<YuvFrame> captureFrame(void) override;






	private:
		
		//��Ƶ֡��ʽ
		typedef struct
		{
			RawImgFmt_t fmt;
			



		}CAP_IMG_FMT;


	private:		
		
		/*
			���ܣ����Զ������Ƶ��ʽת��V4L2��ʶ��ĸ�ʽ
			
			������<commFmt> �Զ���ĸ�ʽ	  
				  
			����ֵ��V4L2��ʶ��ĸ�ʽ
		*/
		UINT32 toV4l2Fmt(RawImgFmt_t commFmt);


		

	private:
		//��Ƶ�ɼ��豸���ļ�������
		int m_deviceFd;

};


class HisiCapture:public AbsFrameCapture
{



};


/*
	capture�����й����࣬��������ϵͳ�����е���Ƶ�ɼ��ࡣ
	Ϊ����ģ���ṩ������Ƶ�ɼ�ͨ���Ľӿڡ�

*/
class CaptureAgent
{
	public:


		/*
			���ܣ���һ��capture�����������м��й���
			
			������	<captureName> capture������
				   <sptrCapture> capture�����ָ��
				  
			����ֵ��true/false
		*/
		bool registerCapture(const std::string & captureName,std::shared_ptr<AbsFrameCapture> sptrCapture);

		/*
			���ܣ���һ��capture����Ӵ�����ɾ��
			
			������	<captureName> capture������				 
				  
			����ֵ��true/false
		*/
		bool unregisterCapture(const std::string & captureName);


		/*
			���ܣ�����һ���Ѿ�ע���caputreָ��
			
			������	<captureName> capture������				 
				  
			����ֵ��caputre�����ָ�룬��������ʱ����ֵΪnullptr
		*/
		std::shared_ptr<AbsFrameCapture> operator[](const std::string & captureName);
		
	
		/*
			���ܣ������Ѿ�ע���capture����
			
			������	void		 
				  
			����ֵ��capture������
		*/
		UINT16 count() const;




	private:
		//�洢����ע���capture
		std::map<std::string,std::shared_ptr<AbsFrameCapture>> m_allCapture;

};













#endif


