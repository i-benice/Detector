#include "capture.h"
#include <sys/types.h>
#include <sys/stat.h>
#include "debug.h"


#define IMG_DEFAULT_WIDTH 	720
#define IMG_DEFAULT_HEIHT 	576

AbsFrameCapture::AbsFrameCapture(const std::string & capturePath):
									m_capturePath(capturePath)
{

}




V4l2Capture::V4l2Capture(const std::string & capturePath):
							AbsFrameCapture(capturePath),
{
	m_deviceFd = -1;
	m_width = IMG_DEFAULT_WIDTH;
	m_height = IMG_DEFAULT_HEIHT;
	m_frameFmt = YUV420_FMT;

}

V4l2Capture::~V4l2Capture()
{


}


bool V4l2Capture::open(void)
{
	int res = -1;
	struct stat fileAttr;			//文件属性
	struct v4l2_format capAttr;		//视频属性设置


	if(m_deviceFd != -1)
	{
		DEBUG_ERR("file (%s) has been opended!",m_capturePath.c_str());
		return true;
	}

	if(m_capturePath.size() == 0)
	{
		return false;
	}
	

	if(stat(m_capturePath.c_str(),&fileAttr) != 0)
	{
		DEBUG_ERR("stat file(%s) failed!",m_capturePath.c_str());
		return false;
	}

	//判断是否为字符设备
	if(!S_ISCHR(m_capturePath.c_str()))
	{
		DEBUG_ERR("open file (%s) failed!,is not a video device",m_capturePath.c_str());
		return false;
	}

	//打开设备
	m_deviceFd = open(m_capturePath.c_str(),O_RDWR|O_NONBLOCK);
	if(m_deviceFd == -1)
	{
		DEBUG_ERR("open file (%s) failed!",m_capturePath.c_str());
		return false;
	}

	//初始化配置视频参数
	memset(&capAttr,0,sizeof(v4l2_format));
	capAttr.fmt.pix.width = m_width; 
	capAttr.fmt.pix.height = m_height; 
	capAttr.fmt.pix.pixelformat = toV4l2Fmt(m_frameFmt);


	return true;
}


bool V4l2Capture::close(void)
{


}



bool V4l2Capture::setFrameRate(UINT16 rate)
{


}


bool V4l2Capture::setFrameSize(UINT16 width,UINT16 height)
{


}


std::shared_ptr<YuvFrame> V4l2Capture::captureFrame(void)
{


}

UINT32 V4l2Capture::toV4l2Fmt(RawImgFmt_t commFmt)
{
	UINT32 v4l2Fmt = 0;
	switch(commFmt)
	{
		case YUV420_FMT:
			v4l2Fmt = 
		case YUV422_FMT:
			v4l2Fmt =
		case YUV444_FMT:
			v4l2Fmt =
		default:
			break;

	}
	return v4l2Fmt;
}







bool CaptureAgent::registerCapture(const std::string & captureName,
										std::shared_ptr<AbsFrameCapture> sptrCapture)
{
	if(sptrCapture == nullptr || captureName.size() == 0)
	{
		return false;
	}

	//插入元素
	auto resPair = m_allCapture.insert(std::pair<std::string,std::shared_ptr<AbsFrameCapture>>(captureName,sptrCapture));

	return resPair.second;

}

bool CaptureAgent::unregisterCapture(const std::string & captureName)
{
	if(captureName.size() == 0)
	{
		return false;
	}
	//删除响应元素
	return m_allCapture.erase(captureName) == 1 ? true : false;
}


std::shared_ptr<AbsFrameCapture> CaptureAgent::operator[](const std::string & captureName)
{
	return m_allCapture[captureName];
}

UINT16 CaptureAgent::count() const
{
	return m_allCapture.size();
}


