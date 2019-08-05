#include <iostream>
#include <tuple>
#include <vector>
#include <cstdint>
#include <random>
#include <algorithm>

//#include "Maze.h"

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

static Maze CreateField(const std::intmax_t& W, const std::intmax_t& H) {
	Maze M(H);

	for (std::size_t i = 0; i < H; i++) {
		M[i].resize(W);
	}
	return M;
}

static bool Show(const Maze& M) {

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

typedef std::tuple < std::intmax_t, std::intmax_t> Point;
typedef std::tuple < Point, std::vector<std::uint8_t>, std::uint8_t> SData;
typedef std::vector<SData> VStack;

template<class Stack,class Map, class Fun, class NFun, class EqualFun, class RE, class Di, class Cood >
bool Search2(Map& M, Cood XY, Di Directions, Fun F, NFun Next, EqualFun IsEq, RE& Random) {
	std::vector<std::uint8_t> DIdx;

	for (std::size_t i = 0; i < Directions.size(); i++) {
		DIdx.push_back(i);
	}

	Stack St;
	auto [CanMove, End] = F(M, XY, XY);
	if (End == true) return true;
	if (!CanMove) {
		return false;
	}

	St.push_back({ XY,DIdx,0 });

	while (St.size()) {
		auto [XY, Dir, i] = St.back();
		St.pop_back();
		for (i; i < DIdx.size(); i++) {
			auto Ne = Next(XY, Directions[Dir[i]]);
			auto [CanMove, End] = F(M, XY, Ne);
			if (End == true) return true;
			if (CanMove) {
				auto It = std::find_if(St.begin(), St.end(), [&](auto& A)->bool { return IsEq(std::get<0>(A), Ne); });
				if (It == St.end()) {
					St.push_back({ XY,Dir,i + 1 });
					std::shuffle(Dir.begin(), Dir.end(), Random);
					St.push_back({ Ne,Dir,0 });
					//i = 0;
				}
			}
		}
	}
	return true;
}
/** /
int main() {
	std::intmax_t LX = 7;
	std::intmax_t LY = 7;
	Maze M = CreateField(LX + 1, LY + 1);
	auto F2 = [&](Maze& M, const Point& XY,const Point& NXY)->std::tuple<bool, bool> {
		std::intmax_t FX = std::get<0>(XY);
		std::intmax_t FY = std::get<1>(XY);		
		std::intmax_t NX = std::get<0>(NXY);
		std::intmax_t NY = std::get<1>(NXY);			
		if (FX == LX && FY == LY)return { false,false };
		if (FX < 0)return { false,false };
		if (FY < 0) return { false,false };
		if (FX >= M.front().size())  return { false,false };
		if (FY >= M.size())  return { false,false };
		if (NX < 0)  return { false,false };
		if (NY < 0) return { false,false };
		if (NX >= M.front().size())return { false,false };
		if (NY >= M.size())return { false,false };

		std::int8_t DX = NX - FX;
		std::int8_t DY = NY - FY;

		if (!((std::abs(DX) == 1 && std::abs(DY) == 0) | (std::abs(DX) == 0 && std::abs(DY) == 1) | (std::abs(DX) == 0 && std::abs(DY) == 0))) return { false,false };
		if (M[NY][NX].Value() != Tyle::All) return { false,false };

		if (DX == 1) {
			M[FY][FX].Drop(Tyle::Right);
			M[NY][NX].Drop(Tyle::Left);
		}
		if (DX == -1) {
			M[FY][FX].Drop(Tyle::Left);
			M[NY][NX].Drop(Tyle::Right);
		}
		if (DY == 1) {
			M[FY][FX].Drop(Tyle::Top);
			M[NY][NX].Drop(Tyle::Bottom);
		}
		if (DY == -1) {
			M[FY][FX].Drop(Tyle::Bottom);
			M[NY][NX].Drop(Tyle::Top);
		}

		return { true,false };
	};
	auto F = [&](Maze& M,const Point& XY, const Point& NXY)->std::tuple<bool, bool> {
		std::intmax_t FX = std::get<0>(XY);
		std::intmax_t FY = std::get<1>(XY);
		std::intmax_t NX = std::get<0>(NXY);
		std::intmax_t NY = std::get<1>(NXY);

		if (FX == NX && FY == NY) return { true,false };

		if (FX == LX && FY == LY) {
			M[FY][FX].Make(1 << 5);
			return { true,true };
		}

		if (FX < 0)  return { false,false };
		if (FY < 0)  return { false,false };
		if (FX >= M.front().size())  return { false,false };
		if (FY >= M.size())  return { false,false };
		if (NX < 0)  return { false,false };
		if (NY < 0)  return { false,false };
		if (NX >= M.front().size())  return { false,false };
		if (NY >= M.size()) return { false,false };


		std::int8_t DX = NX - FX;
		std::int8_t DY = NY - FY;

		M[FY][FX].Make(1 << 5);

		if (DX == 1) {
			if ((M[FY][FX].Value() & Tyle::Right) == 0) return { true ,false };
		}
		if (DX == -1) {
			if ((M[FY][FX].Value() & Tyle::Left) == 0)  return { true ,false };
		}

		if (DY == 1) {
			if ((M[FY][FX].Value() & Tyle::Top) == 0)  return { true ,false };


		}
		if (DY == -1) {
			if ((M[FY][FX].Value() & Tyle::Bottom) == 0)   return { true ,false };
		}



		return { false ,false };;

	};
	auto NF = [](const Point& A, const Point& Dir)->Point {return { std::get<0>(A) + std::get<0>(Dir),std::get<1>(A) + std::get<1>(Dir) }; };
	auto Eq = [](const Point& A, const Point& B)->bool { return (std::get<0>(A) == std::get<0>(B)) && (std::get<1>(A) == std::get<1>(B)); };
	std::vector<Point> Dir = { {1,0},{0,1},{-1,0},{0,-1} };
	Point FS = { 0,0 };
	std::minstd_rand mr(1);
	Search2<VStack>(M, FS, Dir, F,NF,Eq, mr);

	Show(M);

	return 0;
}
/**/