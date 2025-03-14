// tmp_svgimport.hpp

// https://www.w3.org/TR/SVG/paths.html
#include "tinyxml2.h"


namespace h2d {

#if 0

void printFileAttrib( const tinyxml2::XMLDocument& doc )
{
	const tinyxml2::XMLElement* root = doc.RootElement();

	const tinyxml2::XMLAttribute* pAttrib = root->FirstAttribute();
	while(pAttrib)
	{
		std::cout << "Value: " << pAttrib->Name() << ":" << pAttrib->Value() << '\n';
		pAttrib=pAttrib->Next();
	}
}


enum Type {
	Letter,DigitOrDot,Space,MinusOrPlus
};

enum RelMode
{ Relative, Absolute };

/// SVG commands (subset)
enum class Action: uint8_t
{
	LineTo,     ///< L
	Horizontal, ///< H
	Vertical,   ///< V
	MoveTo,     ///< M
	ClosePath   ///< Z
};

//------------------------------------------------------------------
Action getAction( char c )
{
	switch( c )
	{
		case 'L':
		case 'l':
			return Action::LineTo;
		break;
		case 'H':
		case 'h':
			return Action::Horizontal;
		break;
		case 'V':
		case 'v':
			return Action::Vertical;
		break;
		case 'M':
		case 'm':
			return Action::MoveTo;
		break;
		case 'Z':
		case 'z':
			return Action::ClosePath;
		break;
	}
}

Type type( char c )
{
	if( c >= '0' && c <= '9' || c == '.' )
		return DigitOrDot;
	if( c == ' ' )
		return Space;
	if( c == '-' || c == '+' )
		return MinusOrPlus;
	return Letter;
}


double fetchValueAndClear( std::ostringstream& sstr )
{
	double value = std::atof( sstr.str().c_str() );
	std::cout << "val=" << value << '\n';
	sstr.clear();
	sstr.str("");
	return value;
}

bool isAllowedLetter( char c )
{
	if(
		c=='S' || c=='s' || c=='C'  || c=='c'
		|| c=='Q' || c=='q' || c=='T'  || c=='t'
		|| c=='A' || c=='a'
	)
		return false;
	if(
		c=='L' || c=='l' || c=='H' || c=='h'
		|| c=='V' || c=='v' || c=='M' || c=='m'
		|| c=='Z' || c=='z'
	)
		return true;
	return false;
}

//------------------------------------------------------------------
struct SvgPathData
{
private:
	bool hasNumValue   = false;
	bool hasPointValue = false;
	bool isFirstValue  = true;
	std::vector<Point2d> vpts;
	std::vector<double>  v_values;

	bool isX = true;
	std::ostringstream sstr;
	Point2d pt;
	RelMode _mode;
	Action  _action;
	bool _nextValueIsNeg = false;


public:
	size_t getNbArgs()
	{
		if( _action == Action::Horizontal || _action == Action::Vertical )
			return 1;
		if( _action == Action::ClosePath )
			return 0;
		return 2;
	}

	void addDigit( char current )
	{
		sstr << current;
		hasNumValue = true;
	}

	void processCurrentValue() // we have a value that we want to process
	{
		if( hasNumValue ) // if we have already parsed a num value previously,
		{                 // then store it
			hasNumValue = false;
			auto value = fetchValueAndClear(sstr);
			if( _nextValueIsNeg )
			{
				value = -value;
				_nextValueIsNeg = false;
			}
			v_values.push_back( value );
			if( v_values.size() == getNbArgs() ) // we are done
			{
				switch( v_values.size() )
				{
					case 0:
					break;
					case 1:
						assert( _action == Action::Horizontal || _action == Action::Vertical );
					break;
					case 2:
						pt.set( v_values[0], v_values[1] ); // \todo handle relative !!!
						hasPointValue = true;
					break;
					default:assert(0);
				}
				v_values.clear();
			}
			if( hasPointValue )
			{
				vpts.push_back( pt );
				std::cout << "adding pt " << pt << ", size=" << vpts.size() << '\n';
				hasPointValue = false;
			}
		}
	}
	void processLetter( char current )
	{
		std::cout << "process letter " << current << '\n';
		if( !isAllowedLetter( current ) )
			throw std::runtime_error("invalid letter:" + current );

		_mode = Absolute;
		if( islower(current) )
			_mode = Relative;
		_action = getAction( current );

	}
	void nextValueIsNeg()
	{
		_nextValueIsNeg = true;
	}
};

//------------------------------------------------------------------

void parsePath( const std::string& s )
{
	bool Done = false;
	size_t idx = 0;
	SvgPathData data;
	double value = 0.;
	do
	{
		std::cout << "loop start, idx=" << idx << '\n';
		char current = s[idx];
		switch( type(current) )
		{
			case Letter:
				data.processCurrentValue();
				data.processLetter( current );
			break;
			case DigitOrDot:
				data.addDigit( current );
			break;
			case Space:
				data.processCurrentValue();
			break;
			case MinusOrPlus:
				data.nextValueIsNeg();
				data.processCurrentValue();
			break;

		}
		idx++;
	}
	while( !Done && idx < s.size() );
   }

#endif //0


} // namespace svg

} // namespace h2d
