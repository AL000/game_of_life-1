// ***gol.h****
// Game of life


#ifndef __GOL__
#define __GOL__

#include <map>
#include <vector>

struct Cell
{
  Cell(int x_= 0, int y_= 0):x(x_),y(y_){};
	// to use it as a key
	bool operator()(const Cell &c1, const Cell &c2) const
	{
		return (c1.x!=c2.x)? c1.x<c2.x : c1.y<c2.y;
	}
	
	static bool CellCompare(const Cell& a, const Cell& b)
	{
		return a.x==b.x && a.y==b.y;
	}
	 
	int x;
	int y;
};

struct CellProcessor
{
	bool isCell;
	int  cnt;
	CellProcessor():isCell(false),cnt(0){}
};

typedef std::vector<Cell>	            	Elements;
typedef Elements::const_iterator     		Elements_ci;
typedef std::map<Cell,CellProcessor,Cell>	Element_map;
typedef Element_map::const_iterator     	Element_map_ci;

template <typename AliveStrategy, typename BirthStrategy>
class GameOfLife
{
public:
	void init(const Elements& s)
	{
		//todo, check uniqness of each element
		elements = s;
	}
	
	const Elements& iterate()
	{
		//todo, use for_each loops
		Element_map tmp_map;
		for(Elements_ci i(elements.begin()), e(elements.end()); i!=e ; ++i)
		{
			processElement(*i, tmp_map);
		}
		
		elements.clear();
		
		for(Element_map_ci i(tmp_map.begin()), e(tmp_map.end()); i!=e ; ++i)
		{
			if(aliveStrategy(i->second) || birthStrategy(i->second))
			{
				elements.push_back(i->first);
			}
		}
		
		return elements;
	}
	
private:

	void processElement(const Cell& c, Element_map & m)
	{
	   for (int x = -1; x < 2; ++x)
		  for (int y = -1; y < 2; ++y)
		  {
			 const Cell e(c.x + x, c.y + y);
			 if (x || y)
				++m[e].cnt;
			 else
				m[e].isCell = true;
		 }
	}
	
	Elements	 	elements;
	AliveStrategy 	aliveStrategy;
	BirthStrategy 	birthStrategy;
};

#endif
