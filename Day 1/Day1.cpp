#include <iostream>
#include <cassert>
#include <cstdint>
#include <cmath>
#include <expected>
#include <functional>

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
		const auto sign_buffer = std::invoke([&]() -> std::expected<int, std::string> {
			switch (direction) {
			case 'L': return -1;
			case 'R': return 1;
			default: return std::unexpected{"Invalid direction input. Must be 'L' or 'R'."};
			}
		});
		if (!sign_buffer) {
			return std::unexpected{sign_buffer.error()};
		}
		const int sign = *sign_buffer;

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

void test_dial()
{
	Dial dial;
	assert(dial.number() == 50);
	dial.turn('R', 30);
	assert(dial.number() == 80);
	dial.turn('L', 80);
	assert(dial.number() == 0);
	dial.turn('R', 550);
	assert(dial.number() == 50);
	assert(dial.click_count() == 5);
	dial.turn('R', 50);
	assert(dial.click_count() == 6);
}

int main()
{
	test_dial();
}