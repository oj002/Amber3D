#pragma once
#include <vector>
#include <memory>
#include <sstream>


namespace Amber3D
{
	class Logger
	{
	private:
		class LogHelper;
		friend class LogHelper;
	public:
		enum errorLevel
		{
			LOG_FATAL = 0x1 << 0,
			LOG_ERROR = 0x1 << 1,
			LOG_WARNING = 0x1 << 2,
			LOG_INFO = 0x1 << 3,
			LOG_DEBUG = 0x1 << 4,

			LOG_DEFAULT = LOG_FATAL | LOG_ERROR | LOG_WARNING | LOG_INFO,
			LOG_ALL = ~0x0
		};
		Logger(bool defaultOutput = true, bool addPrefix = true, bool addPostfix = true);
		virtual ~Logger();

		Logger& add(std::ostream* output, unsigned int messageFlags = LOG_DEFAULT);
		Logger& remove(std::ostream* output);
		Logger& setOutputFlags(std::ostream* output, unsigned int newFlags);
		unsigned int getNumOutputs() const;

		template<typename T>
		LogHelper operator<<(const T& value)
		{
			if (m_addPrefix)
			{
				m_buffer << m_prefix;
			}
			m_buffer << value;

			return LogHelper(this);
		}

		LogHelper operator<< (std::ostream& (*fn)(std::ostream& os));

		Logger& fatal();
		Logger& error();
		Logger& warning();
		Logger& info();
		Logger& debug();

		Logger& setFlags(unsigned int flags);
		unsigned int getFlags() const;

		void enablePrefix(bool enable = true) { m_addPrefix = enable; }
		void setPrefix(const std::string& prefix = "[Generic] ") { m_prefix = prefix; }
		void enablePostfix(bool enable = true) { m_addPostfix = enable; }
		void setPostfix(const std::string& postfix = "\n") { m_postfix = postfix; }

	private:
		class LogHelper
		{
		public:
			LogHelper(Logger* parent);
			~LogHelper();

			template<typename T>
			LogHelper& operator<<(const T& value)
			{
				m_parent->m_buffer << value;
				return *this;
			}

			LogHelper& operator<<(std::ostream&(*fn)(std::ostream& os));

		private:
			Logger* m_parent;

		};

		void flush();
		void reset();

		typedef std::pair<unsigned int, std::ostream*> OutputPair;
		typedef std::vector<OutputPair> OutputList;
		typedef OutputList::iterator OutputIterator;

		std::stringstream m_buffer;

		bool m_addPrefix : 1;
		bool m_addPostfix : 1;
		std::string m_prefix;
		std::string m_postfix;

		OutputList m_outputs;
		unsigned int m_flags;
	};

	extern Logger gLog;
}
