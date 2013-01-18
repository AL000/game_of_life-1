
#include <iostream>
#include <map>
#include <set>

struct Cell
{
  int x;
	int y;
	Cell(int x_= 0, int y_= 0):x(x_),y(y_){};
	bool operator()(const Cell &c1, const Cell &c2) const
	{
		return (c1.x!=c2.x)? c1.x<c2.x : c1.y<c2.y;
	}
};

struct CellProcessor
{
	bool isCell;
	int  cnt;
	CellProcessor():isCell(false),cnt(0){}
};
	
typedef std::set<Cell, Cell>            	Element_set;
typedef Element_set::const_iterator     	Element_set_ci;
typedef std::map<Cell,CellProcessor,Cell>	Element_map;
typedef Element_map::const_iterator     	Element_map_ci;

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

void processElement(const Cell& c, Element_map & m)
{
   for (int x=-1; x<2; ++x)
      for (int y=-1; y<2; ++y)
      {
         if (x==0 && y==0)
            continue;

	++m[Cell(c.x+x, c.y+y)].cnt;
     }
}

template <typename A, typename B>
class GameOfLife
{
public:
	void init(const Element_set& s)
	{
		element_set = s;
	}
	
	const Element_set& iterate()
	{
		Element_map tmp_map;
		for(Element_set_ci i(element_set.begin()), e(element_set.end()); i!=e ; ++i)
		{
			tmp_map[*i].isCell = true;
			processElement(*i, tmp_map);
		}
		
		element_set.clear();
		
		for(Element_map_ci i(tmp_map.begin()), e(tmp_map.end()); i!=e ; ++i)
		{
			if(aliveStrategy(i->second) || birthStrategy(i->second))
			{
				element_set.insert(i->first);
			}
		}
		
		return element_set;
	}
private:
	
	Element_set 	element_set;
	A 		aliveStrategy;
	B 		birthStrategy;
};

int main()
{
	Element_set es;
	// Die hard pattern
	{Cell c(2,3);es.insert(c);}
	{Cell c(3,3);es.insert(c);}
	{Cell c(3,2);es.insert(c);}
	{Cell c(7,2);es.insert(c);}
	{Cell c(8,2);es.insert(c);}
	{Cell c(9,2);es.insert(c);}
	{Cell c(8,4);es.insert(c);}
	
	GameOfLife <AliveStrategy, BirthStrategy> gol;
	gol.init(es);
	
	for(int i=0; i<130; ++i)
	{ 
		es = gol.iterate();
		std::cout << "Size = " << es.size() << std::endl;
	}
	return 0;
}
