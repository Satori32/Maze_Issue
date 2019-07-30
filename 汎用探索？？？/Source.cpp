#include <iostream>
#include <tuple>
#include <vector>
#include <cstdint>
#include <random>
#include <algorithm>

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
	std::uint8_t Value()const  {
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

typedef std::tuple < std::intmax_t, std::intmax_t, std::vector<std::uint8_t>,std::uint8_t> Data;//x,y,dir,i.
typedef std::vector<Data> VecStack;

template<class Map, class Fun,class RE>
bool Search(Map& M,std::intmax_t X,std::intmax_t Y, Fun F,RE& Random) {
	std::vector<std::uint8_t> DIdx = { 0,1,2,3 };
	std::vector<std::int8_t> XM{ 0,1,0,-1 };
	std::vector<std::int8_t> YM{ 1,0,-1,0 };
	VecStack St;

	if (!F(M, 0, 0, 0, 0,St)) {
		return false;
	}

	St.push_back({ X,Y,DIdx,0 });

	while (St.size()) {
		auto [X,Y,Dir,i]=St.back();
		St.pop_back();
		for (i; i < DIdx.size(); i++) {
			std::intmax_t NX = X + XM[Dir[i]];
			std::intmax_t NY = Y + YM[Dir[i]];
			if (F(M, X, Y, NX, NY,St)) {
				St.push_back({X,Y,Dir,i});
				auto It = std::find_if(St.begin(), St.end(), [&](auto& A) { return std::get<0>(A) == NX && std::get<1>(A) == NY;});
				if (It == St.end()) {
					std::shuffle(Dir.begin(), Dir.end(), Random);
					St.push_back({ NX,NY,Dir,0 }); 
				}
			}
		}
	}
}

Maze CreateField(const std::intmax_t& W,const std::intmax_t& H) {
	Maze M(H);

	for (std::size_t i = 0; i < H; i++) {
		M[i].resize(W);
	}
	return M;
}

bool Show(const Maze& M) {

	for (std::size_t Y = 0; Y < M.size(); Y++) {
		for(std::size_t i=0;i<3;i++){
			for (std::size_t X = 0; X < M.front().size(); X++) {
				if (i == 0) {
					std::cout << 'W' << ((M[Y][X].Value() & Tyle::Bottom) > 0 ? 'W' : ' ') << 'W';
				}
				if (i == 1) {
					std::cout << ((M[Y][X].Value() & Tyle::Left) > 0 ? 'W' : ' ') << ((M[Y][X].Value() & (1<<5)) > 0 ? '+' : ' ') << ((M[Y][X].Value() & Tyle::Right) > 0 ? 'W' : ' ');
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

Maze MakeHoge() {
	std::intmax_t LX = 7;
	std::intmax_t LY = 7;
	Maze M =CreateField(LX+1,LY+1);

	auto F = [&](Maze& M, const std::intmax_t& FX, const std::intmax_t FY, const std::intmax_t& NX, const std::intmax_t& NY, const VecStack& ST) {
		if (FX == LX&&FY==LY)return false;
		if (FX < 0) return false;
		if (FY < 0)return false;
		if (FX >= M.front().size()) return false;
		if (FY >= M.size())return false;
		if (NX < 0) return false;
		if (NY < 0)return false;
		if (NX >= M.front().size()) return false;
		if (NY >= M.size())return false;

		std::int8_t DX = NX - FX;
		std::int8_t DY = NY - FY;

		if (!((std::abs(DX) == 1 && std::abs(DY) == 0) | (std::abs(DX) == 0 && std::abs(DY) == 1)|(std::abs(DX) == 0 && std::abs(DY) == 0))) { return false; }
		if (M[NY][NX].Value() != Tyle::All) { return false; }

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

		return true;

	};

	std::minstd_rand mr(0);
	Search(M, 0, 0, F,mr);

	Show(M);


	/** /
	char ch[] = "0123456789ABCDEF";
	for (std::size_t i = 0; i < M.size(); i++) {
		for (std::size_t j = 0; j < M.front().size(); j++) {
		
			std::cout << ch[M[i][j].Value()]<<',';
		}
		std::cout << std::endl;
	}
	/*`*/
	return M;
}
int main() {

	Maze M = MakeHoge();
	//Show(M);
	return 0;
}
/** /
int main() {

	Maze M = MakeHoge();
	Show(M);
	std::intmax_t LX = 7;
	std::intmax_t LY = 7;
	auto F = [&](Maze& M, const std::intmax_t& FX, const std::intmax_t FY, const std::intmax_t& NX, const std::intmax_t& NY, const VecStack& ST) {

		if (FX == NX && FY == NY) return true;

		if (FX == LX && FY == LY)return false;

		if (FX < 0) return false;
		if (FY < 0)return false;
		if (FX >= M.front().size()) return false;
		if (FY >= M.size())return false;
		if (NX < 0) return false;
		if (NY < 0)return false;
		if (NX >= M.front().size()) return false;
		if (NY >= M.size())return false;


		std::int8_t DX = NX - FX;
		std::int8_t DY = NY - FY;

		//if (!((std::abs(DX) == 1 && std::abs(DY) == 0) | (std::abs(DX) == 0 && std::abs(DY) == 1) | (std::abs(DX) == 0 && std::abs(DY) == 0))) { return false; }
		//if (M[NY][NX].Value() == Tyle::All) { return false; }
		
		M[FY][FX].Make(1 << 5);
		
		if (DX == 1) {
			if (M[FY][FX].Value() & Tyle::Right) return true;
		}
		if (DX == -1) {	
			if (M[FY][FX].Value() & Tyle::Left) return true;
		}
		if (DY == 1) {	
			if (M[FY][FX].Value() & Tyle::Top) return true;

		}
		if (DY == -1) {
			if (M[FY][FX].Value() & Tyle::Bottom) return true;
		}



		return false;

	};

	std::minstd_rand mr(0);
	Search(M, 0, 0, F,mr);

	Show(M);

	return 0;
}
**/
/** /
int main() {

	Tyle T;

	T.Drop(Tyle::Up|Tyle::Bottom);




	return 0;
}
/**/