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

	void Invoke(); //execute�� ����� �ǹ�

	template <typename Process>
	void AddTask(Process&& process); //����! Task�� Process���� ���� ���þ���!

private:
	std::vector<std::thread> threads;
	//�ִ� ������ ��: �� ���μ��� (= �ھ� * 2)
	//thread pool (�̸� ������ �ִ��� �������� �ʿ��Ҷ����� ������)
	std::map<std::thread::id, std::string> thread_names;
	
	//���� ���� �۾���� ó���ؾߵ� queue�ᵵ �Ǵµ� deque�� queue���� ��¦ ����(?)
	std::deque<std::shared_ptr<Task>> tasks;
	std::mutex task_mutex;
	std::condition_variable condition_var; //���Ǻ��� �Һ�-����������
	//Ư�� �����带 ���ų� ����°� ����
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

		//���� �Դٴ°� �̱� �������� ���̹Ƿ� tasks�� �����ų �ʿ� ����
	}

	//thread�� �������Ѥ��� ���ҽ��Ѿ� �Ǳ� ������ �ϴ� tasks�� ������� �δ°�

	std::unique_lock<std::mutex> lock(task_mutex);
	//lockguard�� ����. �����ɶ� lock, �Ҹ�ɶ� unlock
	//���Ǻ��� ������ unique_lock�� ��ߵ�.

	tasks.push_back(std::make_shared<Task>(process));

	lock.unlock();

	//lock�� unlcok ����: critical section �Ӱ迵��

	condition_var.notify_one(); //�ϳ��� �����
}
