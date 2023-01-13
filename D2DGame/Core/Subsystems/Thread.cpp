#include "stdafx.h"
#include "Thread.h"

Thread::Thread(Context * const context)
	:ISubsystem(context)
{
	thread_count_max = std::thread::hardware_concurrency();
	thread_count = thread_count_max - 1;
	thread_names[std::this_thread::get_id()] = "main";
}

Thread::~Thread()
{
	std::unique_lock<std::mutex> lock(task_mutex);

	is_stopping = true;

	lock.unlock();

	condition_var.notify_all();

	for (auto& thread : threads)
		thread.join();

	threads.clear();
	threads.shrink_to_fit();
}

bool Thread::Initialize()
{
	for (uint i = 0; i < thread_count; i++)
	{
		threads.emplace_back(std::thread(&Thread::Invoke, this));
		thread_names[threads.back().get_id()] = "worker_" + std::to_string(i);
	}

	LOG_INFO_F("%d thread have been created", thread_count);

	return true;
}

void Thread::Invoke()
{
	std::shared_ptr<Task> task;

	while (true)
	{
		std::unique_lock<std::mutex> lock(task_mutex);

		condition_var.wait(lock, [this]() {return !tasks.empty() || is_stopping; });

		if (is_stopping && tasks.empty())
			return;

		task = tasks.front();
		tasks.pop_front();

		lock.unlock();

		task->Execute();
	}
}
