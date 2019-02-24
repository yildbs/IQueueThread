#pragma once

#include <chrono>
#include <thread>
#include <vector>
#include <mutex>
#include <queue>



template<typename InputClass, typename OutputClass>
class IQueueThread;

template<typename InputClass, typename OutputClass>
static void RunThread(void* arg)
{
    (static_cast<IQueueThread<InputClass, OutputClass>*>(arg))->Run();
}

template<typename InputClass, typename OutputClass>
class IQueueThread
{
public:
	IQueueThread()
		: enabled(true) 
		, blocked(false) 
		, capacity(30) 
		, threadname("default")
		, started(false)
	{
	}
	virtual ~IQueueThread() {
	}

public:
	void Start() {
		if (this->started) {
			return;
		}

		this->started = true;
		threads.clear();
		threads.push_back(std::thread(RunThread<InputClass, OutputClass>, this));

		this->Sleep(10);
	}
	void Stop(){
		this->SetEnabled(false);
		this->Join();
	}
	void SetThreadname(std::string name) {
		this->threadname = name;
	}
	void SetCapacity(int capacity) {
		assert(capacity > 0 && "capacity must be bigger than zero");
		this->capacity = capacity;
	}
	//Push
	//return false : 
	//- Frame in Queue not been popped to push new frame
	//return true : 
	//- Frame in Queue been popped to push new frame
	bool Push(InputClass const &t) {
		bool popped = false;
		if (this->enabled){
			this->mtx_queue.lock();
			for (; this->capacity < this->queue.size();) {
				InputClass dummy = this->queue.front();
				this->queue.pop();
			}
			this->queue.push(t);
			this->mtx_queue.unlock();
		}
		return popped;
	}
	void Pop(InputClass& t, int wait_until_ms=0) {
		if (this->enabled){
			InputClass temp;
            for(;;){
                bool popped = false;
                this->mtx_queue.lock();
                if(this->queue.size() > 0){
                    t = this->queue.front();
                    this->queue.pop();
                    popped = true;
                }
                this->mtx_queue.unlock();

                if(popped==true || this->enabled == false){
                    break;
                }

                if(wait_until_ms == 0){
					this->Sleep(5);
                }
                else{
                    this->Sleep(wait_until_ms);
                    break;
                }
            }
		}
	}
	void SetPushNext(std::function<bool(OutputClass)> const & pushnext) {
		this->pushnext = pushnext;
	}
	void SetPushNextBackdoor(std::function<bool(OutputClass)> const & pushnextbackdoor) {
		this->pushnextbackdoor = pushnextbackdoor;
	}
	bool PushNext(const OutputClass& output){
		if (this->enabled){
			return this->pushnext(output);
		}
		return false;
	}
	bool PushNextBackdoor(const OutputClass& output){
		if (this->enabled){
			return this->pushnextbackdoor(output);
		}
		return false;
	}

	bool IsBlocked() {
		return this->blocked;
	}
    virtual void Run() = 0;

private:
	void Join() {
		for (auto& thread : threads) {
			thread.join();
		}
	}
	void SetEnabled(bool enabled) {
		this->enabled = enabled;
	}
	void Sleep(int ms)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(ms));
	}

protected:
	bool started;

	std::string threadname;
	volatile bool blocked;
	int capacity;
    std::vector<std::thread> threads;
    bool enabled;
	std::queue<InputClass> queue;
	std::timed_mutex mtx_queue;

private:
	std::function<bool(OutputClass const &)> pushnext;
	std::function<bool(OutputClass const &)> pushnextbackdoor;
};


