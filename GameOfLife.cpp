// Simple realisation of Game of Life
// http://en.wikipedia.org/wiki/Conway's_Game_of_Life
// Poluyanov Yuliy p.yuliy@gmail.com

#include <gtest/gtest.h>
#include "gol.h"
#include <algorithm>


struct AliveStrategy
{
	bool operator()(const CellProcessor& c)
	{
		return c.isCell && (c.cnt==2 || c.cnt==3); 
	};
};

struct BirthStrategy
{
	bool operator()(const CellProcessor& c)
	{
		return !c.isCell && c.cnt==3; 
	};
};

	TEST(CellTest, EquivalenceTest) {
		Cell a(4,7), b(4,7);
		ASSERT_FALSE(a(a,b)) << " equal elements must return as equivalence false";
		ASSERT_FALSE(a(b,a)) << " equal elements must return as equivalence false";
	}
	
	TEST(StrategyTest, AliveDead) {
		AliveStrategy s;
		CellProcessor cp;
		cp.isCell = true;
		cp.cnt = 1;
		ASSERT_FALSE(s(cp)) << " with cnt=" << cp.cnt << " should not live";
		cp.cnt = 2;
		ASSERT_TRUE(s(cp))  << "with cnt=" << cp.cnt << " should live";
		cp.cnt = 3;
		ASSERT_TRUE(s(cp))  << "with cnt=" << cp.cnt << " should live";
		cp.cnt = 4;
		ASSERT_FALSE(s(cp)) << "with cnt=" << cp.cnt << " should not live";
	}

	TEST(GOLTest, Beacon) {
		GameOfLife <AliveStrategy, BirthStrategy> gol;
		
		Elements b;
		{Cell c(5,2);b.push_back(c);}
		{Cell c(5,3);b.push_back(c);}
		{Cell c(5,4);b.push_back(c);}
		std::sort(b.begin(),b.end(),Cell());
			
		Elements e;
		{Cell c(4,3);e.push_back(c);}
		{Cell c(5,3);e.push_back(c);}
		{Cell c(6,3);e.push_back(c);}
		
		gol.init(e);
		Elements c = gol.iterate();
		std::sort(c.begin(),c.end(),Cell());
		
		EXPECT_TRUE(std::equal(c.begin(),c.end(),b.begin(),&Cell::CellCompare))
			<< " Test of oscillator failed";
		for(int i=0;i<1000;++i)
		{
			const Elements& e = gol.iterate();
			EXPECT_EQ(e.size(),3);
		}
	}

#if 1
int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

#else
int main(int argc, char **argv)
{
	Elements es;
	
	// endless life
	{Cell c(2,2);es.push_back(c);}
	{Cell c(4,3);es.push_back(c);}
	{Cell c(4,2);es.push_back(c);}
	{Cell c(6,4);es.push_back(c);}
	{Cell c(6,5);es.push_back(c);}
	{Cell c(6,6);es.push_back(c);}
	{Cell c(8,5);es.push_back(c);}
	{Cell c(8,6);es.push_back(c);}
	{Cell c(8,7);es.push_back(c);}
	{Cell c(9,6);es.push_back(c);}

	// Beacon
	/*{Cell c(4,3);es.push_back(c);}
	{Cell c(4,4);es.push_back(c);}
	{Cell c(3,3);es.push_back(c);}
	{Cell c(3,4);es.push_back(c);}
	
	{Cell c(1,1);es.push_back(c);}
	{Cell c(2,1);es.push_back(c);}
	{Cell c(1,2);es.push_back(c);}
	{Cell c(2,2);es.push_back(c);}*/
	
	GameOfLife <AliveStrategy, BirthStrategy> gol;
	gol.init(es);
	
	for(int i=0; ; ++i)
	{ 
		const Elements &es = gol.iterate();
		std::cout << i << " Size = " << es.size() << std::endl;
	}
	return 0;
}
#endif
