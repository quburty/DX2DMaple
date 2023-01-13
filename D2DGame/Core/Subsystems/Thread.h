#pragma once
#include "ISubsystem.h"

class Task final
{
public:
	using Process = std::function<void()>;

public:
	Task(Process&& process)
	{
		this->process = std::forward<Process>(process);
	}

	~Task() = default;

	void Execute()
	{
		process();
	}

private:
	Process process;
};

class Thread final : public ISubsystem
{
public:
	Thread(class Context* const context);
	~Thread();

	bool Initialize() override;

	auto GetThreadCount() const -> const uint& { return thread_count; }
	auto GetThreadCountMax() const -> const uint& { return thread_count_max; }

	void Invoke(); //execute랑 비슷한 의미

	template <typename Process>
	void AddTask(Process&& process); //주의! Task의 Process랑은 전혀 관련없음!

private:
	std::vector<std::thread> threads;
	//최대 쓰레드 수: 논리 프로세스 (= 코어 * 2)
	//thread pool (미리 쓰레드 최대한 만들어놓고 필요할때마다 꺼내씀)
	std::map<std::thread::id, std::string> thread_names;
	
	//먼저 들어온 작업대로 처리해야됨 queue써도 되는데 deque가 queue보다 살짝 빠름(?)
	std::deque<std::shared_ptr<Task>> tasks;
	std::mutex task_mutex;
	std::condition_variable condition_var; //조건변수 소비-생산자패턴
	//특정 쓰레드를 재우거나 깨우는게 가능
	uint thread_count_max = 0;
	uint thread_count = 0;
	bool is_stopping = false;
};

template<typename Process>
inline void Thread::AddTask(Process && process)
{
	if (threads.empty())
	{
		LOG_WARNING("No available threads, function will execute in the same thread");
		process(); // ?????
		return;

		//여기 왔다는건 싱글 쓰레드라는 뜻이므로 tasks에 저장시킬 필요 없음
	}

	//thread가 있을때ㅡㄴㄴ 분할시켜야 되기 때문에 일단 tasks에 저장시켜 두는거

	std::unique_lock<std::mutex> lock(task_mutex);
	//lockguard와 유사. 생성될때 lock, 소멸될때 unlock
	//조건변수 쓸려면 unique_lock을 써야됨.

	tasks.push_back(std::make_shared<Task>(process));

	lock.unlock();

	//lock과 unlcok 사이: critical section 임계영역

	condition_var.notify_one(); //하나만 깨우기
}
