#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <stack>
#include <queue>

//Kevin Andersen
//Ticket producer consumer
using namespace std;

const int n = 5;
int m_front = 0, m_rear = 0;
int m_empty = n;
int m_ticket = 0, m_nextTicket = 0;

int buffer[n];

mutex* producerlock = new mutex;
mutex* consumerlock = new mutex;

void producer()
{
	auto threadID = this_thread::get_id();

	while (true)
	{
		while (m_empty == 0)
		{
			cout << "Producer Spining. Thread: " << threadID << endl;
			this_thread::sleep_for(chrono::seconds(2));
		}

		producerlock->lock();
		if (m_empty != 0)
		{
			m_empty -= 1;
			producerlock->unlock();

			producerlock->lock();
			buffer[m_rear] = m_ticket++;
			cout << buffer[m_rear] << " deposited the food. Thread: " << threadID << endl;
			m_rear = (m_rear + 1) % n;
			producerlock->unlock();
		}
		else
		{
			producerlock->unlock();
		}
	}

	cout << "Production Finished" << endl;
}

void consumer()
{
	auto threadID = this_thread::get_id();

	while (true)
	{
		int ticket = m_nextTicket++;

		while (ticket != buffer[m_front])
		{
			continue;
		}

		this_thread::sleep_for(chrono::seconds(1));
		consumerlock->lock();
		cout << buffer[m_front] << " ate the food. Thread: " << threadID << endl;
		m_front = (m_front + 1) % n;
		consumerlock->unlock();

		m_empty += 1;
	}
}

int main(void)
{
	std::vector<std::thread*> producerThreads;
	std::vector<std::thread*> consumerThreads;

	for (int i = 0; i < n; i++)
	{
		buffer[i] = -1;
	}

	for (int i = 0; i < 3; i++)
	{
		std::thread producer(producer);
		producer.detach();
		producerThreads.push_back(&producer);

		std::thread consumer(consumer);
		consumer.detach();
		consumerThreads.push_back(&consumer);
	}

	cin.get();
	return 0;
}