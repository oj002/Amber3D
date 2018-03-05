#pragma once

namespace Amber3D
{
	template<size_t size>
	struct BooleanArray
	{
		bool get(size_t index)
		{
			switch (index % 4)
			{
			case 0:
				return m_data[index / 4].b0;
				break;
			case 1:
				return m_data[index / 4].b1;
				break;

			case 2:
				return m_data[index / 4].b2;
				break;

			case 3:
				return m_data[index / 4].b3;
				break;
			}
			return false;
		}

		void set(size_t index, bool val)
		{
			switch (index % 4)
			{
			case 0:
				m_data[index / 4].b0 = val;
				break;
			case 1:
				m_data[index / 4].b1 = val;
				break;

			case 2:
				m_data[index / 4].b2 = val;
				break;

			case 3:
				m_data[index / 4].b3 = val;
				break;
			}
		}
	private:
		struct boolsX4
		{
			bool b0 : 1, b1 : 1, b2 : 1, b3 : 1;
		} m_data[size / 4];
	};
} // namespace Amber3D