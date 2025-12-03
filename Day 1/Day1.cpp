#include <iostream>
#include <cstdint>
#include <cmath>
#include <expected>

class Dial {
public:
	size_t number() const
	{
		return m_number;
	}

	size_t click_count() const
	{
		return m_click_count;
	}

	auto turn(const char direction, const size_t amount)
	-> std::expected<void, std::string>
	{
		int sign;
		switch (direction) {
		case 'L':
			sign = -1;
			break;
		case 'R':
			sign = 1;
			break;
		default:
			return std::unexpected{"Invalid direction input. Must be 'L' or 'R'."};
		}

		const size_t subtraction = m_number + sign * amount;
		const size_t clamp = subtraction % max_number;
		const size_t click_count = std::abs(std::floor(subtraction / 100.0f));

		m_number = clamp;
		m_click_count += click_count;

		return {};
	}

private:
	const size_t max_number = 100;

	size_t m_number{50};
	size_t m_click_count{0};
};

int main()
{
	Dial dial;
	std::cout << dial.number() << " -> ";
	dial.turn('R', 30);
	std::cout << dial.number() << '\n';
}