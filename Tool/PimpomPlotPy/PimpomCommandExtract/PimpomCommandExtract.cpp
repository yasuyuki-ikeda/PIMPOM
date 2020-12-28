/*******************************************************************
Copyright (c) 2020, yasuyuki ikeda All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
* Redistributions of source code must retain the above copyright notice, 
  this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice, 
  this list of conditions and the following disclaimer in the documentation 
  and/or other materials provided with the distribution.
* Neither the name of the <organization> nor the names of its contributors 
  may be used to endorse or promote products derived from this software 
  without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************/
//

#include "stdafx.h"
#include "PIMPOM_API.h"




//マクロ定義をCPPファイル出力する
void save_macro_definishion(CString filename)
{
	CCalculate	calclate;

	//calclate.ClearAll();

	FILE *fp;
	fopen_s(&fp, filename, "wt");
	if (fp)
	{
		//プロトタイプ実装
		fprintf(fp, "#include \"stdafx.h\"");
		fprintf(fp, "\n");
		fprintf(fp, "int _execute_command(std::string command, std::string &result);");
		fprintf(fp, "\n");
		fprintf(fp, "\n");


		//関数実装
		for (int i = 0; i < CALC_MACRO_CNT_MAX; i++)
		{
			MACRO_DEF_STRUCT def = { 0 };
			calclate.GetMacroDefinision(i, &def);
			if (def.name[0] == 0)	continue;


			static char argtype[][256] = { "double", "std::string" };
			int is_string_ret = def.ret_type != COM_TYPE_NUMBER;
			fprintf(fp, "static %s _%s(", argtype[is_string_ret], def.name);//関数名

			//関数名の引数
			for (int n = 0; n < def.args[0]; n++)
			{
				int is_string_arg = (def.arg_type[0][n] != COM_TYPE_NUMBER);
				fprintf(fp, "%s arg%d", argtype[is_string_arg], n);

				if (n < def.args[0] - 1){
					fprintf(fp, ",");
				}
			}
			fprintf(fp, "){\n");

			//関数内部
			fprintf(fp, "     std::string command;");
			fprintf(fp, "\n");
			fprintf(fp, "     std::string result;");
			fprintf(fp, "\n");
			fprintf(fp, "\n");

			fprintf(fp, "     command += \"%s(\";", def.name);
			fprintf(fp, "\n");

			//引数
			for (int n = 0; n < def.args[0]; n++)
			{
				int is_string_arg = (def.arg_type[0][n] != COM_TYPE_NUMBER);
				if (is_string_arg)
				{
					fprintf(fp, "     command += std::string(\"\\\"\") + std::string(arg%d) + std::string(\"\\\"\"); ", n);
				}
				else{
					fprintf(fp, "     command += std::to_string(arg%d);", n);
				}
				fprintf(fp, "\n");

				if (n < def.args[0] - 1){
					fprintf(fp, "     command += \",\";");
					fprintf(fp, "\n");
				}
			}
			fprintf(fp, "     command += \")\";");
			fprintf(fp, "\n");
			fprintf(fp, "\n");


			fprintf(fp, "     _execute_command(command, result);");//コマンド実行
			fprintf(fp, "\n");

			//戻り値
			if (is_string_ret){
				fprintf(fp, "     return result;");
			}
			else{
				fprintf(fp, "     return stod(result);");
			}
			fprintf(fp, "\n");

			fprintf(fp, "}");
			fprintf(fp, "\n");
			fprintf(fp, "\n");
			fprintf(fp, "\n");
		}


		//関数登録
		fprintf(fp, "void pimpom_api_def(){");
		fprintf(fp, "\n");
		for (int i = 0; i < CALC_MACRO_CNT_MAX; i++)
		{
			MACRO_DEF_STRUCT def = { 0 };
			calclate.GetMacroDefinision(i, &def);
			if (def.name[0] == 0)	continue;

			fprintf(fp, "     p::def(\"%s\", _%s);", def.name, def.name);
			fprintf(fp, "\n");
		}
		fprintf(fp, "}\n");

		fclose(fp);
	}
}





int _tmain(int argc, _TCHAR* argv[])
{
	save_macro_definishion("..\\..\\..\\Tool\\PimpomPlotPy\\PimpomPlotPy\\pimpom_macro_def.cpp");



	return 0;
}

