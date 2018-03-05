#include "Logger.hpp"
#include <iostream>

#if __has_include(<Windows.h>)
#include <Windows.h>
#define setConsoleTextColor(color) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
#else
#define setConsoleTextColor(color) 
#endif

namespace Amber3D
{
	Logger::Logger(bool defaultOutput, bool addPrefix, bool addPostfix)
		: m_flags(LOG_DEFAULT)
		, m_addPrefix(addPrefix)
		, m_addPostfix(addPostfix)
		, m_prefix("[Generic] ")
		, m_postfix ("\n")
	{
		m_buffer.clear();

		if (defaultOutput)
		{
			add(&std::cout);
		}
	}

	Logger::~Logger()
	{}

	Logger & Logger::add(std::ostream* output, unsigned int messageFlags)
	{
		m_outputs.push_back(std::make_pair(messageFlags, output));
		return *this;
	}

	Logger & Logger::remove(std::ostream * output)
	{
		for (OutputIterator it = m_outputs.begin(); it != m_outputs.end(); ++it)
		{
			if (it->second == output)
			{
				m_outputs.erase(it);
				break;
			}
		}
		return *this;
	}

	Logger & Logger::setOutputFlags(std::ostream * output, unsigned int newFlags)
	{
		for (OutputIterator it = m_outputs.begin(); it != m_outputs.end(); ++it)
		{
			if (it->second == output)
			{
				it->first = newFlags;
				break;
			}
		}
		return *this;
	}

	unsigned int Logger::getNumOutputs() const
	{
		return static_cast<unsigned int>(m_outputs.size());
	}

	Logger::LogHelper Logger::operator<<(std::ostream &(*fn)(std::ostream &os))
	{
		std::ostringstream oss;
		fn(oss);
		return (LogHelper(this) << oss.str());
	}

	Logger & Logger::fatal()
	{
		m_flags = LOG_FATAL;
		return *this;
	}

	Logger & Logger::error()
	{
		m_flags = LOG_ERROR;
		return *this;
	}

	Logger & Logger::warning()
	{
		m_flags = LOG_WARNING;
		return *this;
	}

	Logger & Logger::info()
	{
		m_flags = LOG_INFO;
		return *this;
	}

	Logger & Logger::debug()
	{
		m_flags = LOG_DEBUG;
		return *this;
	}

	Logger & Logger::setFlags(unsigned int flags)
	{
		m_flags = flags;
		return *this;
	}

	unsigned int Logger::getFlags() const
	{
		return m_flags;
	}

	void Logger::flush()
	{
		for (OutputIterator it = m_outputs.begin(); it != m_outputs.end(); ++it)
		{
			if (it->first & m_flags)
			{
				if (it->second == &std::cout)
				{
					int color = 7;
					switch (m_flags)
					{
					case LOG_FATAL: { color = 13; }
					break;
					case LOG_ERROR: { color = 12; }
					break;
					case LOG_WARNING: { color = 11; }
					break;
					case LOG_INFO: { color = 7; }
					break;
					case LOG_DEBUG: { color = 6; }
					break;
					}
					setConsoleTextColor(color);
					(*(it->second)) << m_buffer.str();
					setConsoleTextColor(7);
				}
				else
				{
					(*(it->second)) << m_buffer.str();
				}
			}
		}
		m_buffer.str("");
		reset();
	}

	void Logger::reset()
	{
		m_flags = LOG_DEFAULT;
	}

	Logger::LogHelper::LogHelper(Logger * parent)
		: m_parent(parent)
	{
	}

	Logger::LogHelper::~LogHelper()
	{
		if (m_parent->m_addPostfix)
		{
			m_parent->m_buffer << m_parent->m_postfix;
		}
		m_parent->flush();
	}

	Logger::LogHelper & Logger::LogHelper::operator<<(std::ostream &(*fn)(std::ostream &os))
	{
		std::ostringstream oss;
		fn(oss);
		return ((*this) << oss.str());
	}

	Logger gLog(true);
}
