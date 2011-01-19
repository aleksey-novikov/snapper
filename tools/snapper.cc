
#include <stdlib.h>
#include <getopt.h>
#include <iostream>

#include <snapper/Snapper.h>
#include <snapper/Snapshot.h>
#include <snapper/File.h>
#include <snapper/AppUtil.h>
#include <snapper/SnapperTmpl.h>
#include <snapper/Compare.h>

using namespace snapper;
using namespace std;

typedef void (*cmd_fnc)( const list<string>& args );
map<string,cmd_fnc> cmds;

int print_number = 0;

void showHelp( const list<string>& args )
    {
    cout << 
"Usage: snapper [-h] [ command [ params ] ] ...\n"
"\n"
"Possible commands are:\n"
"    help                 -- show this help\n"
"    list                 -- list all snapshots\n"
"    create single [desc] -- create single snapshot with \"descr\" as description\n"
"    create pre [desc]    -- create pre snapshot with \"descr\" as description\n"
"    create post num      -- create post snapshot that corresponds to\n"
"                            pre snapshot number \"num\"\n"
"    diff num1 num2       -- show difference between snapnots num1 and num2.\n"
"                            current version of filesystem is indicated by number 0.\n"
"\n"
"It is possible to have multiple commands on one command line.\n";
    }

void listSnap( const list<string>& args )
    {
    snapshotlist.assertInit();

    for (vector<Snapshot>::const_iterator it = snapshotlist.begin();
	 it != snapshotlist.end(); ++it)
	{
	cout << *it << endl;
	}
    }

void createSnap( const list<string>& args )
    {
    unsigned int number1 = 0;
    string type;
    string desc;
    list<string>::const_iterator s = args.begin();
    if( s!=args.end() )
	type = *s++;
    if( s!=args.end() )
	{
	if( type == "post" )
	    *s >> number1;
	else
	    desc = *s;
	++s;
	}
    y2mil( "type:" << type << " desc:\"" << desc << "\" number1:" << number1 );
    if( type=="single" )
    {
	number1 = snapshotlist.createSingleSnapshot(desc);
	if (print_number)
	    cout << number1 << endl;
    }
    else if( type=="pre" )
    {
	number1 = snapshotlist.createPreSnapshot(desc);
	if (print_number)
	    cout << number1 << endl;
    }
    else if( type=="post" )
    {
	unsigned int number2 = snapshotlist.createPostSnapshot(number1);
	if (print_number)
	    cout << number2 << endl;
	startBackgroundComparsion(number1, number2);
    }
    else
	y2war( "unknown type:\"" << type << "\"" );
    }

void showDifference( const list<string>& args )
    {
    unsigned n1 = 0;
    unsigned n2 = 0;
    list<string>::const_iterator s = args.begin();
    if( s!=args.end() )
	{
	if( *s != "CURRENT" )
	    *s >> n1;
	++s;
	}
    if( s!=args.end() )
	{
	if( *s != "CURRENT" )
	    *s >> n2;
	++s;
	}
    y2mil( "n1:" << n1 << " n2:" << n2 );

    setComparisonNums(n1, n2);

    for (vector<File>::const_iterator it = filelist.begin(); it != filelist.end(); ++it)
	cout << statusToString(it->getPreToPostStatus()) << " " << it->getName() << endl;
    }

int
main(int argc, char** argv)
    {
    list<string> args;
    initDefaultLogger();
    y2mil( "argc:" << argc );

    static struct option long_options[] = {
	{ "help", 0, 0, 'h' },
	{ "print-number", 0, &print_number, 1 },
	{ 0, 0, 0, 0 }
    };
    int ch;
    while( (ch=getopt_long( argc, argv, "h", long_options, NULL )) != -1 )
	{
	switch( ch )
	    {
	    case 'h':
		showHelp(args);
		exit(0);
		break;
	    default:
		break;
	    }
	}

    cmds["list"] = listSnap;
    cmds["help"] = showHelp;
    cmds["create"] = createSnap;
    cmds["diff"] = showDifference;

    int cnt = optind;
    while( cnt<argc )
	{
	map<string, cmd_fnc>::const_iterator c = cmds.find(argv[cnt]);
	if( c != cmds.end() )
	    {
	    list<string> args;
	    while( ++cnt<argc && cmds.find(argv[cnt])==cmds.end() )
		args.push_back(argv[cnt]);
	    (*c->second)(args);
	    }
	else 
	    {
	    y2war( "Unknown command: \"" << argv[cnt] << "\"" );
	    cerr << "Unknown command: \"" << argv[cnt] << "\"" << endl;
	    ++cnt;
	    }
	}
    exit(EXIT_SUCCESS);
}