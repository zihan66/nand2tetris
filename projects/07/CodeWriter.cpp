#include "CodeWriter.h"
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

// Constructor
CodeWriter::CodeWriter(string filename)
{
	filename_ = filename;
	filename = filename + ".asm";
	outf.open(filename.c_str());
	if (!outf.is_open())
		cerr << "Could not open output file " << filename_ << ".\n";
}

// Destructor
CodeWriter::~CodeWriter()
{
	if (outf.is_open())
		outf.close();
}

// Methods
void CodeWriter::setFileName(string filename)
{
	if (outf.is_open())
		outf.close();
	filename_ = filename;
	filename = filename + ".asm";
	outf.open(filename.c_str());
	if (!outf.is_open())
		cerr << "Could not open output file " << filename_ << ".\n";
}

void CodeWriter::writeArithmetic(string command)
{
	if (command == "add")
	{
		outf << "@SP" << endl << "M=M-1" << endl << "A=M" << endl << "D=M" << endl << "@SP" << endl << "M=M-1" << endl << "A=M" << endl << "M=D+M" << endl << "@SP" << endl << "M=M+1" << endl;
	}
	else if (command == "sub")
	{
		outf << "@SP" << endl << "M=M-1" << endl << "A=M" << endl << "D=M" << endl << "@SP" << endl << "M=M-1" << endl << "A=M" << endl << "M=M-D" << endl << "@SP" << endl << "M=M+1" << endl;
	}
	else if (command == "neg")
	{
		outf << "@SP" << endl << "M=M-1" << endl << "A=M" << endl << "M=-M" << endl << "@SP" << endl << "M=M+1" << endl;
	}
	else if (command == "not")
	{
		outf << "@SP" << endl << "M=M-1" << endl << "A=M" << endl << "M=!M" << endl << "@SP" << endl << "M=M+1" << endl;
	}
	else if (command == "and")
	{
		outf << "@SP" << endl << "M=M-1" << endl << "A=M" << endl << "D=M" << endl << "@SP" << endl << "M=M-1" << endl << "A=M" << endl << "M=D&M" << endl << "@SP" << endl << "M=M+1" << endl;
	}
	else if (command == "or")
	{
		outf << "@SP" << endl << "M=M-1" << endl << "A=M" << endl << "D=M" << endl << "@SP" << endl << "M=M-1" << endl << "A=M" << endl << "M=D|M" << endl << "@SP" << endl << "M=M+1" << endl;
	}
	else if (command == "eq")
	{
		outf << "@SP" << endl << "M=M-1" << endl << "A=M" << endl << "D=M" << endl << "@SP" << endl << "M=M-1" << endl << "A=M" << endl << "D=D-M" << endl << "@EQ" << endl << "D;JEQ" << endl <<
			"@NEQ" << endl << "D;JNE" << endl << "(EQ)" << endl << "@SP" << endl << "A=M" << endl << "M=-1" << endl << "@SP" << endl << "M=M+1" << endl << "(NEQ)" << "@SP" << endl << "A=M" << endl
			<< "M=0" << endl << "@SP" << endl << "M=M+1" << endl;
	}
	else if (command == "gt")
	{
		outf << "@SP" << endl << "M=M-1" << endl << "A=M" << endl << "D=M" << endl << "@SP" << endl << "M=M-1" << endl << "A=M" << endl << "D=D-M" << endl << "@GT" << endl << "D;JLT" << endl <<
			"@NGT" << endl << "D;JGE" << endl << "(GT)" << endl << "@SP" << endl << "A=M" << endl << "M=-1" << endl << "@SP" << endl << "M=M+1" << endl << "(NGT)" << "@SP" << endl << "A=M" << endl
			<< "M=0" << endl << "@SP" << endl << "M=M+1" << endl;
	}
	else if (command == "lt")
	{
		outf << "@SP" << endl << "M=M-1" << endl << "A=M" << endl << "D=M" << endl << "@SP" << endl << "M=M-1" << endl << "A=M" << endl << "D=D-M" << endl << "@LT" << endl << "D;JGT" << endl <<
			"@NLT" << endl << "D;JLE" << endl << "(LT)" << endl << "@SP" << endl << "A=M" << endl << "M=-1" << endl << "@SP" << endl << "M=M+1" << endl << "(NLT)" << "@SP" << endl << "A=M" << endl
			<< "M=0" << endl << "@SP" << endl << "M=M+1" << endl;
	}
}

void CodeWriter::writePushPop(VMcommand pushOrPop, string segment, int index)
{
	if (pushOrPop == C_PUSH)
	{
		if (segment == "constant")
		{
			outf << "@" << index << endl << "D=A" << endl << "@SP" << endl << "A=M" << endl << "M=D" << endl << "@SP" << endl << "M=M+1" << endl;
		}
		else if (segment == "local")
		{
			outf << "@LCL" << endl << "D=M" << endl << "@" << index << endl << "A=D+A" << endl << "D=M" << endl << "@SP" << endl << "A=M" << endl << "M=D" << endl << "@SP" << endl << "M=M+1" << endl;
		}
		else if (segment == "argument")
		{
			outf << "@ARG" << endl << "D=M" << endl << "@" << index << endl << "A=D+A" << endl << "D=M" << endl << "@SP" << endl << "A=M" << endl << "M=D" << endl << "@SP" << endl << "M=M+1" << endl;
		}
		else if (segment == "this")
		{
			outf << "@THIS" << endl << "D=M" << endl << "@" << index << endl << "A=D+A" << endl << "D=M" << endl << "@SP" << endl << "A=M" << endl << "M=D" << endl << "@SP" << endl << "M=M+1" << endl;
		}
		else if (segment == "that")
		{
			outf << "@THAT" << endl << "D=M" << endl << "@" << index << endl << "A=D+A" << endl << "D=M" << endl << "@SP" << endl << "A=M" << endl << "M=D" << endl << "@SP" << endl << "M=M+1" << endl;
		}
		else if (segment == "pointer")
		{
			outf << "@" << index << endl << "D=A" << endl << "@3" << endl << "A=D+A" << endl << "D=M" << endl << "@SP" << endl << "A=M" << endl << "M=D" << endl << "@SP" << endl << "M=M+1" << endl;
		}
		else if (segment == "temp")
		{
			outf << "@" << index << endl << "D=A" << endl << "@5" << endl << "A=D+A" << endl << "D=M" << endl << "@SP" << endl << "A=M" << endl << "M=D" << endl << "@SP" << endl << "M=M+1" << endl;
		}
		else if (segment == "static")
		{
			outf << "@" << filename_ << "." << index << endl << "D=M" << endl << "@SP" << endl << "A=M" << endl << "M=D" << endl << "@SP" << endl << "M=M+1" << endl;
		}
	}
	else if (pushOrPop == C_POP)
	{
		if (segment == "local")
		{
			outf << "@SP" << endl << "M=M-1" << endl << "A=M" << endl << "D=M" << endl << "@R13" << endl << "M=D" << endl << "@LCL" << endl << "D=M" << endl << "@" << index << endl << "D=A+D" << endl
				<< "@R14" << endl << "M=D" << endl << "@R13" << endl << "D=M" << endl << "@R14" << endl << "A=M" << endl << "M=D" << endl;
		}

	}
}