#include <iostream>
#include <cassert>
#include <cstdint>
#include <cmath>
#include <expected>
#include <functional>

class Dial {
public:
	enum class Direction { Left, Right };

	struct Instruction {
		Direction direction;
		size_t amount;
	};

	size_t number() const
	{
		return m_number;
	}

	size_t click_count() const
	{
		return m_click_count;
	}

	void set(size_t number)
	{
		m_number = number;
	}

	void turn(const Direction direction, const size_t amount)
	{
		const auto sign = std::invoke([&]() -> int {
			switch (direction) {
			case Direction::Left: return -1;
			case Direction::Right: return 1;
			}
			return 0; // Avoid no-default-case warning.
		});

		const size_t subtraction = m_number + sign * amount;
		const size_t clamp = subtraction % max_number;
		const size_t click_count = std::abs(std::floor(subtraction / 100.0f));

		m_number = clamp;
		m_click_count += click_count;
	}

	void sequence_turns(std::vector<Instruction>& instructions)
	{
		for (auto& instruction : instructions) {
			turn(instruction.direction, instruction.amount);
		}
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
	dial.turn(Dial::Direction::Right, 30);
	assert(dial.number() == 80);
	dial.turn(Dial::Direction::Left, 80);
	assert(dial.number() == 0);
	dial.turn(Dial::Direction::Right, 550);
	assert(dial.number() == 50);
	assert(dial.click_count() == 5);
	dial.turn(Dial::Direction::Right, 50);
	assert(dial.click_count() == 6);
}

int main()
{
	test_dial();
}