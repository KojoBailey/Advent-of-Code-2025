#include <iostream>
#include <cassert>
#include <cstdint>
#include <cmath>
#include <expected>
#include <functional>
#include <fstream>
#include <vector>

class Dial {
public:
	enum class Direction { Left, Right };

	struct Instruction {
		Direction direction;
		size_t amount;
	};

	[[nodiscard]] size_t number() const
	{
		return m_number;
	}

	[[nodiscard]] size_t zero_count() const
	{
		return m_zero_count;
	}

	[[nodiscard]] size_t click_count() const
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
		if (m_number == 0) {
			m_zero_count++;
		}
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
	size_t m_zero_count{0};
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
	assert(dial.zero_count() == 1);
	dial.turn(Dial::Direction::Right, 550);
	assert(dial.number() == 50);
	assert(dial.click_count() == 5);
	dial.turn(Dial::Direction::Right, 50);
	assert(dial.click_count() == 6);
}

auto string_to_instruction(std::string_view raw)
-> std::expected<Dial::Instruction, std::string>
{
	const auto direction_buffer = std::invoke([&]() -> std::expected<Dial::Direction, std::string> {
		const char direction_char = raw[0];
		switch (direction_char) {
		case 'L': return Dial::Direction::Left;
		case 'R': return Dial::Direction::Right;
		}
		return std::unexpected{std::format(
			"Invalid direction '{}'. Must be 'L' or 'R'.", direction_char
		)};
	});
	if (!direction_buffer) {
		return std::unexpected{direction_buffer.error()};
	}
	const Dial::Direction direction = *direction_buffer;

	const size_t amount = std::stoi(std::string{raw}.substr(1, raw.length() - 1));

	return Dial::Instruction{direction, amount};
}

void test_string_to_instruction()
{
	const auto left_buffer = string_to_instruction("L67");
	assert(left_buffer.has_value());
	assert(left_buffer.value().direction == Dial::Direction::Left);
	assert(left_buffer.value().amount == 67);
	const auto right_buffer = string_to_instruction("R3");
	assert(right_buffer.has_value());
	assert(right_buffer.value().direction == Dial::Direction::Right);
	assert(right_buffer.value().amount == 3);
	const auto false_buffer = string_to_instruction("XXX");
	assert(!false_buffer.has_value());
}

auto parse_instructions(std::vector<std::string>& raws)
-> std::expected<std::vector<Dial::Instruction>, std::string>
{
	std::vector<Dial::Instruction> result;

	for (auto& raw : raws) {
		const auto buffer = string_to_instruction(raw);
		if (!buffer) {
			return std::unexpected{buffer.error()};
		}
		result.push_back(*buffer);
	}

	return result;
}

int main()
{
	test_dial();
	test_string_to_instruction();

	std::ifstream file{"./Day 1/input.txt"};
	if (!file.is_open()) {
		std::cerr << "Unable to open file.\n";
		return 1;
	}

	std::vector<std::string> raw_instructions;
	std::string buffer;
	while (!file.eof()) {
		std::getline(file, buffer);
		if (buffer != "") {
			raw_instructions.push_back(buffer);
		}
	}

	auto parsed_instructions_buffer = parse_instructions(raw_instructions);
	if (!parsed_instructions_buffer) {
		std::cerr << parsed_instructions_buffer.error();
	}
	std::vector<Dial::Instruction> parsed_instructions = std::move(*parsed_instructions_buffer);

	Dial dial;
	dial.sequence_turns(parsed_instructions);
	std::cout << dial.zero_count();
}