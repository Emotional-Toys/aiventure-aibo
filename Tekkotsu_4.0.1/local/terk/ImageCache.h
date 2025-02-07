#ifndef INCLUDED_ImageCache_h
#define INCLUDED_ImageCache_h

#include <unistd.h>
#include <map>
#include <iostream>
#include <fstream>

#include "TeRKPeerCommon.h"
#include "Shared/get_time.h"
#include "IPC/Thread.h"

#include "local/DataSource.h"

class ImageCache : public DataSource{
public:
	ImageCache() : lastTime(0), fps(0), number(0), currentBuffer(0), frameLock() {}
	virtual ~ImageCache() {}
	virtual void newImage(::TeRK::ByteArray data);
	
	virtual unsigned int nextTimestamp();
	virtual const std::string& nextName();
	virtual unsigned int getData(const char *& payload, unsigned int& size, unsigned int& timestamp, std::string& name);
	virtual void setDataSourceThread(LoadDataThread* p);
	
protected:
	class Frame {
	public:
		Frame() : number(), lifetime(0), data(NULL), size(0), unread(false), width(0), height(0), components(0) {}

		~Frame() {
			if(this->data)
				delete [] this->data;
		}
		
		void setData(::TeRK::ByteArray dataArray, int number);
		
		int number;
		
		float lifetime;
		char* data;
		unsigned int size;
		bool unread;
		
		size_t width;
		size_t height;
		size_t components;
		
		size_t getHeaderSize() const;
		size_t writeHeader(char* buf, size_t size) const;

	private:
		Frame(const Frame&); //!< do not call
		Frame& operator=(const Frame&); //!< do not call
		
	};
	
	unsigned int lastTime;
	unsigned int fps;
	unsigned int number;
	
	int currentBuffer;
	Frame buffers[2];
	
	ThreadNS::Lock frameLock;
};

#endif
