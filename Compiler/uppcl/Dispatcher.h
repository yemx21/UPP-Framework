#pragma once
#include <string>
#include <iostream>
#include <fstream>

namespace uppcl
{
	class Dispatcher
	{
	protected:
		bool slience_;
	public:
		Dispatcher();

		void SetSlience(bool slience);
		bool GetSlience() const;

		virtual void SendError(const std::string& file, int line, int column, const std::string& msg, const std::string& project);
		virtual void SendError(const std::string& file, int line, int column, const std::string& msg);

		virtual void SendTip(const std::string& file, int line, int column, const std::string& msg, const std::string& project);
		virtual void SendTip(const std::string& file, int line, int column, const std::string& msg);
		
		virtual void SendWarning(const std::string& file, int line, int column, const std::string& msg, const std::string& project);
		virtual void SendWarning(const std::string& file, int line, int column, const std::string& msg);
		
		virtual void SendOutput(const std::string& msg, const std::string& project);
		virtual void SendOutput(const std::string& msg);

	};

	class DiskWriter: public Dispatcher
	{
	private:
		std::ofstream writer;
	public:
		DiskWriter();
		~DiskWriter();

		bool Open(const std::string& path);

		void SendError(const std::string& file, int line, int column, const std::string& msg, const std::string& project) override;
		void SendError(const std::string& file, int line, int column, const std::string& msg) override;
		
		void SendTip(const std::string& file, int line, int column, const std::string& msg, const std::string& project) override;
		void SendTip(const std::string& file, int line, int column, const std::string& msg) override;

		void SendWarning(const std::string& file, int line, int column, const std::string& msg, const std::string& project) override;
		void SendWarning(const std::string& file, int line, int column, const std::string& msg) override;

		void SendOutput(const std::string& msg, const std::string& project) override;
		void SendOutput(const std::string& msg) override;
	};

	class StdWriter : public Dispatcher
	{
	public:
		StdWriter();
		~StdWriter();

		void SendError(const std::string& file, int line, int column, const std::string& msg, const std::string& project) override;
		void SendError(const std::string& file, int line, int column, const std::string& msg) override;

		void SendTip(const std::string& file, int line, int column, const std::string& msg, const std::string& project) override;
		void SendTip(const std::string& file, int line, int column, const std::string& msg) override;

		void SendWarning(const std::string& file, int line, int column, const std::string& msg, const std::string& project) override;
		void SendWarning(const std::string& file, int line, int column, const std::string& msg) override;

		void SendOutput(const std::string& msg, const std::string& project) override;
		void SendOutput(const std::string& msg) override;
	};
}