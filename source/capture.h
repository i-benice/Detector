#ifndef _VIDEO_CAPTURE_H
#define _VIDEO_CAPTURE_H


#include <string>
#include <memory>
#include <map>
#include "frame.h"
#include "dataType.h"
#include "videoComm.h"







/*
	视频采集抽象基类
*/
class AbsFrameCapture
{
	public:
		AbsFrameCapture(const std::string & capturePath);
		virtual ~AbsFrameCapture() = default;

		/*
		功能：打开视频设备

		参数：void

		返回值：true/false
		*/
		virtual bool open(void)=0;



		/*
		功能：关闭视频设备

		参数：void

		返回值：true/false
		*/
		virtual bool close(void)=0;

		
		/*
			功能：设置摄像头采集帧率

			参数：<rate>	 帧率，每秒钟捕获的视频帧个数

			返回值：true/false
		*/
		virtual bool setFrameRate(UINT16 rate) = 0;
		
		/*
			功能：设置视频帧尺寸

			参数：<width>	  图像的宽度像素值
				  <height> 图像的高度像素值
				  
			返回值：true/false
		*/
		virtual bool setFrameSize(UINT16 width,UINT16 height)=0;



		/*
			功能：设置视频帧格式

			参数：<imgFormat>	视频帧的格式				  
				  
			返回值：true/false
		*/
		virtual bool setFrameFmt(RawImgFmt_t imgFormat)=0;


		/*
			功能：从底层获取一帧图像
			
			参数：void				  
				  
			返回值：Yuv帧的指针,如果为空则代表采集帧失败
		*/
		virtual std::shared_ptr<YuvFrame> captureFrame(void) = 0;



		
	protected:

		//视频源名称
		std::string m_capturePath;

		//图像宽度像素值
		UINT16 m_width;

		//图像高度像素值
		UINT16 m_height;

		//图像格式
		RawImgFmt_t m_frameFmt;

};


class V4l2Capture:public AbsFrameCapture
{
	public:
		
	

	public:
		V4l2Capture(const std::string & capturePath);
		virtual ~V4l2Capture();

		/*
		功能：打开视频设备.在类的构造函数中默认调用open操作

		参数：void

		返回值：true/false
		*/
		virtual bool open(void) override;



		/*
		功能：关闭视频设备

		参数：void

		返回值：true/false
		*/
		virtual bool close(void) override;


		
		/*
		功能：设置摄像头采集帧率

		参数：<rate>	 帧率，每秒钟捕获的视频帧个数

		返回值：true/false
		*/
		virtual bool setFrameRate(UINT16 rate) override;
		
		/*
			功能：设置视频帧尺寸

			参数：<width>	  图像的宽度像素值
				  <height> 图像的高度像素值
				  
			返回值：true/false
		*/
		virtual bool setFrameSize(UINT16 width,UINT16 height) override;



		
		/*
			功能：设置视频帧格式

			参数：<imgFormat>	视频帧的格式				  
				  
			返回值：true/false
		*/
		virtual bool setFrameFmt(RawImgFmt_t imgFormat) override;


		/*
			功能：从底层获取一帧图像
			
			参数：void				  
				  
			返回值：Yuv帧的指针,如果为空则代表采集帧失败
		*/
		virtual std::shared_ptr<YuvFrame> captureFrame(void) override;






	private:
		
		//视频帧格式
		typedef struct
		{
			RawImgFmt_t fmt;
			



		}CAP_IMG_FMT;


	private:		
		
		/*
			功能：将自定义的视频格式转成V4L2能识别的格式
			
			参数：<commFmt> 自定义的格式	  
				  
			返回值：V4L2能识别的格式
		*/
		UINT32 toV4l2Fmt(RawImgFmt_t commFmt);


		

	private:
		//视频采集设备的文件描述符
		int m_deviceFd;

};


class HisiCapture:public AbsFrameCapture
{



};


/*
	capture对象集中管理类，用来管理系统中所有的视频采集类。
	为其他模块提供控制视频采集通道的接口。

*/
class CaptureAgent
{
	public:


		/*
			功能：将一个capture对象放入代理中集中管理
			
			参数：	<captureName> capture的名称
				   <sptrCapture> capture对象的指针
				  
			返回值：true/false
		*/
		bool registerCapture(const std::string & captureName,std::shared_ptr<AbsFrameCapture> sptrCapture);

		/*
			功能：将一个capture对象从代理中删除
			
			参数：	<captureName> capture的名称				 
				  
			返回值：true/false
		*/
		bool unregisterCapture(const std::string & captureName);


		/*
			功能：返回一个已经注册的caputre指针
			
			参数：	<captureName> capture的名称				 
				  
			返回值：caputre对象的指针，当不存在时返回值为nullptr
		*/
		std::shared_ptr<AbsFrameCapture> operator[](const std::string & captureName);
		
	
		/*
			功能：返回已经注册的capture总数
			
			参数：	void		 
				  
			返回值：capture的数量
		*/
		UINT16 count() const;




	private:
		//存储所有注册的capture
		std::map<std::string,std::shared_ptr<AbsFrameCapture>> m_allCapture;

};













#endif


