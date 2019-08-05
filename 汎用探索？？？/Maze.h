#pragma once
#include <cstdint>
#include <vector>

class Tyle {
public:
	enum {
		None = 0,
		Top = 1,
		Right = 2,
		Bottom = 4,
		Left = 8,
		All = 15,
	};
	std::uint8_t Value()const {
		return Walls;
	}
	bool Make(const std::uint8_t& W) {
		Walls |= W;
		return true;
	}
	bool Drop(const std::uint8_t W) {
		Walls &= (~W);
		return true;
	}
protected:
	std::uint8_t Walls = All;
};

typedef std::vector<std::vector<Tyle>> Maze;

Maze CreateField(const std::intmax_t& W, const std::intmax_t& H) {
	Maze M(H);

	for (std::size_t i = 0; i < H; i++) {
		M[i].resize(W);
	}
	return M;
}

bool Show(const Maze& M) {

	for (std::size_t Y = 0; Y < M.size(); Y++) {
		for (std::size_t i = 0; i < 3; i++) {
			for (std::size_t X = 0; X < M.front().size(); X++) {
				if (i == 0) {
					std::cout << 'W' << ((M[Y][X].Value() & Tyle::Bottom) > 0 ? 'W' : ' ') << 'W';
				}
				if (i == 1) {
					std::cout << ((M[Y][X].Value() & Tyle::Left) > 0 ? 'W' : ' ') << ((M[Y][X].Value() & (1 << 5)) > 0 ? '+' : ' ') << ((M[Y][X].Value() & Tyle::Right) > 0 ? 'W' : ' ');
				}
				if (i == 2) {
					std::cout << 'W' << ((M[Y][X].Value() & Tyle::Top) > 0 ? 'W' : ' ') << 'W';
				}
			}
			std::cout << std::endl;
		}
	}
	return true;
}