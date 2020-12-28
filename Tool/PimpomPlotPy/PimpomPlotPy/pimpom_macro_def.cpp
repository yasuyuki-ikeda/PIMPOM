#include "stdafx.h"
int _execute_command(std::string command, std::string &result);

static double _I(double arg0){
     std::string command;
     std::string result;

     command += "I(";
     command += std::to_string(arg0);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _sin(double arg0){
     std::string command;
     std::string result;

     command += "sin(";
     command += std::to_string(arg0);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _cos(double arg0){
     std::string command;
     std::string result;

     command += "cos(";
     command += std::to_string(arg0);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _tan(double arg0){
     std::string command;
     std::string result;

     command += "tan(";
     command += std::to_string(arg0);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _log(double arg0){
     std::string command;
     std::string result;

     command += "log(";
     command += std::to_string(arg0);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _sqrt(double arg0){
     std::string command;
     std::string result;

     command += "sqrt(";
     command += std::to_string(arg0);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _exp(double arg0){
     std::string command;
     std::string result;

     command += "exp(";
     command += std::to_string(arg0);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _abs(double arg0){
     std::string command;
     std::string result;

     command += "abs(";
     command += std::to_string(arg0);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _random(){
     std::string command;
     std::string result;

     command += "random(";
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _rand(){
     std::string command;
     std::string result;

     command += "rand(";
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _ceil(double arg0){
     std::string command;
     std::string result;

     command += "ceil(";
     command += std::to_string(arg0);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _floor(double arg0){
     std::string command;
     std::string result;

     command += "floor(";
     command += std::to_string(arg0);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _asin(double arg0){
     std::string command;
     std::string result;

     command += "asin(";
     command += std::to_string(arg0);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _acos(double arg0){
     std::string command;
     std::string result;

     command += "acos(";
     command += std::to_string(arg0);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _pow(double arg0,double arg1){
     std::string command;
     std::string result;

     command += "pow(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _mod(double arg0,double arg1){
     std::string command;
     std::string result;

     command += "mod(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _atan2(double arg0,double arg1){
     std::string command;
     std::string result;

     command += "atan2(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _atan(double arg0){
     std::string command;
     std::string result;

     command += "atan(";
     command += std::to_string(arg0);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _gaus(){
     std::string command;
     std::string result;

     command += "gaus(";
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _thresh(double arg0,double arg1){
     std::string command;
     std::string result;

     command += "thresh(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _delta(double arg0,double arg1){
     std::string command;
     std::string result;

     command += "delta(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _A(double arg0,double arg1,double arg2,double arg3,double arg4){
     std::string command;
     std::string result;

     command += "A(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _M(double arg0){
     std::string command;
     std::string result;

     command += "M(";
     command += std::to_string(arg0);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _min(double arg0,double arg1){
     std::string command;
     std::string result;

     command += "min(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _max(double arg0,double arg1){
     std::string command;
     std::string result;

     command += "max(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _PI(){
     std::string command;
     std::string result;

     command += "PI(";
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _dist(double arg0,double arg1,double arg2,double arg3){
     std::string command;
     std::string result;

     command += "dist(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _sigmoid(double arg0,double arg1){
     std::string command;
     std::string result;

     command += "sigmoid(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _ReLU(double arg0){
     std::string command;
     std::string result;

     command += "ReLU(";
     command += std::to_string(arg0);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _GetDataUnitNumber(){
     std::string command;
     std::string result;

     command += "GetDataUnitNumber(";
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _GetCurrentImageNumber(){
     std::string command;
     std::string result;

     command += "GetCurrentImageNumber(";
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _SetCurrentImageNumber(double arg0){
     std::string command;
     std::string result;

     command += "SetCurrentImageNumber(";
     command += std::to_string(arg0);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _GetDataTypeSize(double arg0,double arg1){
     std::string command;
     std::string result;

     command += "GetDataTypeSize(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _CopyDataUnit(double arg0,double arg1){
     std::string command;
     std::string result;

     command += "CopyDataUnit(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _DeleteDataUnit(double arg0){
     std::string command;
     std::string result;

     command += "DeleteDataUnit(";
     command += std::to_string(arg0);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _BackupDataUnit(double arg0){
     std::string command;
     std::string result;

     command += "BackupDataUnit(";
     command += std::to_string(arg0);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _RecoverDataUnit(double arg0){
     std::string command;
     std::string result;

     command += "RecoverDataUnit(";
     command += std::to_string(arg0);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _ConvertDataUnit(double arg0,double arg1,double arg2,double arg3,double arg4,double arg5,double arg6){
     std::string command;
     std::string result;

     command += "ConvertDataUnit(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ",";
     command += std::to_string(arg6);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _ConvertDispToByte(double arg0){
     std::string command;
     std::string result;

     command += "ConvertDispToByte(";
     command += std::to_string(arg0);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static std::string _GetSrcFilePathName(double arg0){
     std::string command;
     std::string result;

     command += "GetSrcFilePathName(";
     command += std::to_string(arg0);
     command += ")";

     _execute_command(command, result);
     return result;
}


static std::string _GetSrcFilePath(double arg0){
     std::string command;
     std::string result;

     command += "GetSrcFilePath(";
     command += std::to_string(arg0);
     command += ")";

     _execute_command(command, result);
     return result;
}


static std::string _GetSrcFileName(double arg0){
     std::string command;
     std::string result;

     command += "GetSrcFileName(";
     command += std::to_string(arg0);
     command += ")";

     _execute_command(command, result);
     return result;
}


static std::string _GetSrcFileTitle(double arg0){
     std::string command;
     std::string result;

     command += "GetSrcFileTitle(";
     command += std::to_string(arg0);
     command += ")";

     _execute_command(command, result);
     return result;
}


static double _FormatDataUnit(double arg0,double arg1,double arg2,double arg3,double arg4){
     std::string command;
     std::string result;

     command += "FormatDataUnit(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _GetDataMinMax(double arg0,double arg1){
     std::string command;
     std::string result;

     command += "GetDataMinMax(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _SetDispPage(double arg0,double arg1){
     std::string command;
     std::string result;

     command += "SetDispPage(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _SetDispChannel(double arg0,double arg1){
     std::string command;
     std::string result;

     command += "SetDispChannel(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _UseWorkArea(double arg0,double arg1){
     std::string command;
     std::string result;

     command += "UseWorkArea(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _DrawImage(double arg0,std::string arg1,double arg2){
     std::string command;
     std::string result;

     command += "DrawImage(";
     command += std::to_string(arg0);
     command += ",";
     command += std::string("\"") + std::string(arg1) + std::string("\""); 
     command += ",";
     command += std::to_string(arg2);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _ScaleImage(double arg0,double arg1){
     std::string command;
     std::string result;

     command += "ScaleImage(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _SetDispRange(double arg0,double arg1,double arg2){
     std::string command;
     std::string result;

     command += "SetDispRange(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _SetDispRangeAuto(double arg0,double arg1){
     std::string command;
     std::string result;

     command += "SetDispRangeAuto(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _SetWorkArea(double arg0,double arg1,double arg2,double arg3,double arg4,double arg5,double arg6){
     std::string command;
     std::string result;

     command += "SetWorkArea(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ",";
     command += std::to_string(arg6);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _GetWorkAreaVtx(double arg0,double arg1,double arg2){
     std::string command;
     std::string result;

     command += "GetWorkAreaVtx(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _GetWorkAreaOrnot(double arg0,double arg1){
     std::string command;
     std::string result;

     command += "GetWorkAreaOrnot(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _ChangePalette(double arg0,double arg1){
     std::string command;
     std::string result;

     command += "ChangePalette(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _AddMark(double arg0,double arg1,double arg2){
     std::string command;
     std::string result;

     command += "AddMark(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _DeleteMark(double arg0,double arg1){
     std::string command;
     std::string result;

     command += "DeleteMark(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _GetMarkVal(double arg0,double arg1,double arg2){
     std::string command;
     std::string result;

     command += "GetMarkVal(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _GetMarkNumber(double arg0){
     std::string command;
     std::string result;

     command += "GetMarkNumber(";
     command += std::to_string(arg0);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _ClearMark(double arg0){
     std::string command;
     std::string result;

     command += "ClearMark(";
     command += std::to_string(arg0);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _AddLineMark(double arg0,double arg1,double arg2,double arg3,double arg4){
     std::string command;
     std::string result;

     command += "AddLineMark(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _ResizeDataUnit(double arg0,double arg1,double arg2,double arg3,double arg4,double arg5,double arg6,double arg7){
     std::string command;
     std::string result;

     command += "ResizeDataUnit(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ",";
     command += std::to_string(arg6);
     command += ",";
     command += std::to_string(arg7);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _ClipDataUnit(double arg0,double arg1,double arg2,double arg3,double arg4,double arg5){
     std::string command;
     std::string result;

     command += "ClipDataUnit(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _PerspectiveTransDataUnit(double arg0,double arg1,double arg2,double arg3,double arg4,double arg5,double arg6,double arg7,double arg8,double arg9,double arg10,double arg11){
     std::string command;
     std::string result;

     command += "PerspectiveTransDataUnit(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ",";
     command += std::to_string(arg6);
     command += ",";
     command += std::to_string(arg7);
     command += ",";
     command += std::to_string(arg8);
     command += ",";
     command += std::to_string(arg9);
     command += ",";
     command += std::to_string(arg10);
     command += ",";
     command += std::to_string(arg11);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _IsoTransDataUnit(double arg0,double arg1,double arg2,double arg3,double arg4,double arg5,double arg6){
     std::string command;
     std::string result;

     command += "IsoTransDataUnit(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ",";
     command += std::to_string(arg6);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _CalcDataValue(double arg0,double arg1,double arg2,double arg3,std::string arg4,double arg5){
     std::string command;
     std::string result;

     command += "CalcDataValue(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::string("\"") + std::string(arg4) + std::string("\""); 
     command += ",";
     command += std::to_string(arg5);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _CalcMaskValue(double arg0,std::string arg1,double arg2,double arg3){
     std::string command;
     std::string result;

     command += "CalcMaskValue(";
     command += std::to_string(arg0);
     command += ",";
     command += std::string("\"") + std::string(arg1) + std::string("\""); 
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _FilterSmooth(double arg0,double arg1,double arg2,double arg3,double arg4,double arg5){
     std::string command;
     std::string result;

     command += "FilterSmooth(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _FilterGauss(double arg0,double arg1,double arg2,double arg3,double arg4,double arg5){
     std::string command;
     std::string result;

     command += "FilterGauss(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _FilterGaussUnshade(double arg0,double arg1,double arg2,double arg3,double arg4,double arg5,double arg6,double arg7){
     std::string command;
     std::string result;

     command += "FilterGaussUnshade(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ",";
     command += std::to_string(arg6);
     command += ",";
     command += std::to_string(arg7);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _FilterMedian(double arg0,double arg1,double arg2,double arg3,double arg4,double arg5){
     std::string command;
     std::string result;

     command += "FilterMedian(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _FilterErosion(double arg0,double arg1,double arg2,double arg3,double arg4,double arg5){
     std::string command;
     std::string result;

     command += "FilterErosion(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _FilterDilation(double arg0,double arg1,double arg2,double arg3,double arg4,double arg5){
     std::string command;
     std::string result;

     command += "FilterDilation(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _FilterOpening(double arg0,double arg1,double arg2,double arg3,double arg4,double arg5){
     std::string command;
     std::string result;

     command += "FilterOpening(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _FilterClosing(double arg0,double arg1,double arg2,double arg3,double arg4,double arg5){
     std::string command;
     std::string result;

     command += "FilterClosing(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _FilterEdgeSobel(double arg0,double arg1,double arg2,double arg3,double arg4,double arg5){
     std::string command;
     std::string result;

     command += "FilterEdgeSobel(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _FilterLaplaceGauss(double arg0,double arg1,double arg2,double arg3,double arg4,double arg5,double arg6,double arg7){
     std::string command;
     std::string result;

     command += "FilterLaplaceGauss(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ",";
     command += std::to_string(arg6);
     command += ",";
     command += std::to_string(arg7);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _FilterEdgeLog(double arg0,double arg1,double arg2,double arg3,double arg4,double arg5,double arg6){
     std::string command;
     std::string result;

     command += "FilterEdgeLog(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ",";
     command += std::to_string(arg6);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _FilterMirror(double arg0,double arg1,double arg2,double arg3){
     std::string command;
     std::string result;

     command += "FilterMirror(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _FilterNega(double arg0,double arg1,double arg2){
     std::string command;
     std::string result;

     command += "FilterNega(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _FilterEdgeCanny(double arg0,double arg1,double arg2,double arg3,double arg4,double arg5){
     std::string command;
     std::string result;

     command += "FilterEdgeCanny(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _FilterBilateral(double arg0,double arg1,double arg2,double arg3,double arg4,double arg5){
     std::string command;
     std::string result;

     command += "FilterBilateral(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _FilterGabor(double arg0,double arg1,double arg2,double arg3,double arg4,double arg5,double arg6,double arg7,double arg8,double arg9,double arg10){
     std::string command;
     std::string result;

     command += "FilterGabor(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ",";
     command += std::to_string(arg6);
     command += ",";
     command += std::to_string(arg7);
     command += ",";
     command += std::to_string(arg8);
     command += ",";
     command += std::to_string(arg9);
     command += ",";
     command += std::to_string(arg10);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _FilterEdgeSigmoid(double arg0,double arg1,double arg2,double arg3,double arg4,double arg5,double arg6,double arg7,double arg8){
     std::string command;
     std::string result;

     command += "FilterEdgeSigmoid(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ",";
     command += std::to_string(arg6);
     command += ",";
     command += std::to_string(arg7);
     command += ",";
     command += std::to_string(arg8);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _FilterCLAHE(double arg0,double arg1,double arg2,double arg3,double arg4,double arg5){
     std::string command;
     std::string result;

     command += "FilterCLAHE(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _FilterLocalContrast(double arg0,double arg1,double arg2,double arg3,double arg4){
     std::string command;
     std::string result;

     command += "FilterLocalContrast(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _FilterLocalContrastPatch(double arg0,double arg1,double arg2,double arg3){
     std::string command;
     std::string result;

     command += "FilterLocalContrastPatch(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _Binalize(double arg0,double arg1,double arg2,double arg3,double arg4){
     std::string command;
     std::string result;

     command += "Binalize(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _BinalizeThresholdOtsu(double arg0){
     std::string command;
     std::string result;

     command += "BinalizeThresholdOtsu(";
     command += std::to_string(arg0);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _BinalizeThresholdPTile(double arg0,double arg1){
     std::string command;
     std::string result;

     command += "BinalizeThresholdPTile(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _BinalizeLocalGauss(double arg0,double arg1,double arg2,double arg3,double arg4,double arg5){
     std::string command;
     std::string result;

     command += "BinalizeLocalGauss(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _BinalizeLocalLaplaceGauss(double arg0,double arg1,double arg2,double arg3,double arg4,double arg5){
     std::string command;
     std::string result;

     command += "BinalizeLocalLaplaceGauss(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _BinalizeNick(double arg0,double arg1,double arg2,double arg3,double arg4){
     std::string command;
     std::string result;

     command += "BinalizeNick(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _Label(double arg0,double arg1,double arg2,double arg3){
     std::string command;
     std::string result;

     command += "Label(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _HoughLines(double arg0,double arg1,double arg2,double arg3,double arg4,double arg5){
     std::string command;
     std::string result;

     command += "HoughLines(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _BlendPoisson(double arg0,double arg1,double arg2,double arg3,double arg4,double arg5){
     std::string command;
     std::string result;

     command += "BlendPoisson(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _PrepeareDrawing(double arg0,double arg1){
     std::string command;
     std::string result;

     command += "PrepeareDrawing(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _DrawLineOnImage(double arg0,double arg1,double arg2,double arg3,double arg4,double arg5,double arg6,double arg7,double arg8){
     std::string command;
     std::string result;

     command += "DrawLineOnImage(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ",";
     command += std::to_string(arg6);
     command += ",";
     command += std::to_string(arg7);
     command += ",";
     command += std::to_string(arg8);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _DrawCursorOnImage(double arg0,double arg1,double arg2,double arg3,double arg4,double arg5,double arg6){
     std::string command;
     std::string result;

     command += "DrawCursorOnImage(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ",";
     command += std::to_string(arg6);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _DrawCircleOnImage(double arg0,double arg1,double arg2,double arg3,double arg4,double arg5,double arg6,double arg7){
     std::string command;
     std::string result;

     command += "DrawCircleOnImage(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ",";
     command += std::to_string(arg6);
     command += ",";
     command += std::to_string(arg7);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _DrawRectOnImage(double arg0,double arg1,double arg2,double arg3,double arg4,double arg5,double arg6,double arg7,double arg8){
     std::string command;
     std::string result;

     command += "DrawRectOnImage(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ",";
     command += std::to_string(arg6);
     command += ",";
     command += std::to_string(arg7);
     command += ",";
     command += std::to_string(arg8);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _DrawPointOnImage(double arg0,double arg1,double arg2,double arg3,double arg4,double arg5,double arg6){
     std::string command;
     std::string result;

     command += "DrawPointOnImage(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ",";
     command += std::to_string(arg6);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _DrawTextOnImage(double arg0,double arg1,double arg2,std::string arg3,double arg4,double arg5,double arg6){
     std::string command;
     std::string result;

     command += "DrawTextOnImage(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::string("\"") + std::string(arg3) + std::string("\""); 
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ",";
     command += std::to_string(arg6);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _FloodFillOnImage(double arg0,double arg1,double arg2,double arg3,double arg4,double arg5){
     std::string command;
     std::string result;

     command += "FloodFillOnImage(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _FillWorkAreaOnImage(double arg0,double arg1,double arg2,double arg3){
     std::string command;
     std::string result;

     command += "FillWorkAreaOnImage(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _DrawLineOnImageF(double arg0,double arg1,double arg2,double arg3,double arg4,double arg5,double arg6,double arg7){
     std::string command;
     std::string result;

     command += "DrawLineOnImageF(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ",";
     command += std::to_string(arg6);
     command += ",";
     command += std::to_string(arg7);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _DrawImageOnImage(double arg0,double arg1,double arg2,double arg3,double arg4){
     std::string command;
     std::string result;

     command += "DrawImageOnImage(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _FillWorkAreaOnMask(double arg0,double arg1){
     std::string command;
     std::string result;

     command += "FillWorkAreaOnMask(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _DrawLine(double arg0,double arg1,double arg2,double arg3,double arg4,double arg5,double arg6,double arg7){
     std::string command;
     std::string result;

     command += "DrawLine(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ",";
     command += std::to_string(arg6);
     command += ",";
     command += std::to_string(arg7);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _DrawCursor(double arg0,double arg1,double arg2,double arg3,double arg4,double arg5){
     std::string command;
     std::string result;

     command += "DrawCursor(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _DrawRect(double arg0,double arg1,double arg2,double arg3,double arg4,double arg5,double arg6,double arg7){
     std::string command;
     std::string result;

     command += "DrawRect(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ",";
     command += std::to_string(arg6);
     command += ",";
     command += std::to_string(arg7);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _DrawCircle(double arg0,double arg1,double arg2,double arg3,double arg4,double arg5,double arg6){
     std::string command;
     std::string result;

     command += "DrawCircle(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ",";
     command += std::to_string(arg6);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _DrawEllipse(double arg0,double arg1,double arg2,double arg3,double arg4,double arg5,double arg6,double arg7){
     std::string command;
     std::string result;

     command += "DrawEllipse(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ",";
     command += std::to_string(arg6);
     command += ",";
     command += std::to_string(arg7);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _DrawText(double arg0,double arg1,std::string arg2,double arg3,double arg4,double arg5,double arg6){
     std::string command;
     std::string result;

     command += "DrawText(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::string("\"") + std::string(arg2) + std::string("\""); 
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ",";
     command += std::to_string(arg6);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _ClipSegment(double arg0,double arg1,double arg2,double arg3,double arg4,double arg5,double arg6,double arg7,double arg8){
     std::string command;
     std::string result;

     command += "ClipSegment(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ",";
     command += std::to_string(arg6);
     command += ",";
     command += std::to_string(arg7);
     command += ",";
     command += std::to_string(arg8);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _ClipSegmentF(double arg0,double arg1,double arg2,double arg3,double arg4,double arg5,double arg6,double arg7){
     std::string command;
     std::string result;

     command += "ClipSegmentF(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ",";
     command += std::to_string(arg6);
     command += ",";
     command += std::to_string(arg7);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _ShowMessage(double arg0,double arg1,std::string arg2){
     std::string command;
     std::string result;

     command += "ShowMessage(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::string("\"") + std::string(arg2) + std::string("\""); 
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _MessageBox(std::string arg0){
     std::string command;
     std::string result;

     command += "MessageBox(";
     command += std::string("\"") + std::string(arg0) + std::string("\""); 
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _CopyToClipbord(double arg0){
     std::string command;
     std::string result;

     command += "CopyToClipbord(";
     command += std::to_string(arg0);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _PasteFromClipbord(double arg0){
     std::string command;
     std::string result;

     command += "PasteFromClipbord(";
     command += std::to_string(arg0);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static std::string _ToString(double arg0,double arg1){
     std::string command;
     std::string result;

     command += "ToString(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ")";

     _execute_command(command, result);
     return result;
}


static double _ToNumber(std::string arg0){
     std::string command;
     std::string result;

     command += "ToNumber(";
     command += std::string("\"") + std::string(arg0) + std::string("\""); 
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _GetBatchInput(double arg0){
     std::string command;
     std::string result;

     command += "GetBatchInput(";
     command += std::to_string(arg0);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _SetBatchOutput(double arg0,double arg1){
     std::string command;
     std::string result;

     command += "SetBatchOutput(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _SetBatchInput(double arg0,double arg1){
     std::string command;
     std::string result;

     command += "SetBatchInput(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _GetBatchOutput(double arg0){
     std::string command;
     std::string result;

     command += "GetBatchOutput(";
     command += std::to_string(arg0);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _UserDlgBatchExecute(double arg0){
     std::string command;
     std::string result;

     command += "UserDlgBatchExecute(";
     command += std::to_string(arg0);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _CaptureOne(){
     std::string command;
     std::string result;

     command += "CaptureOne(";
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _SendString(std::string arg0){
     std::string command;
     std::string result;

     command += "SendString(";
     command += std::string("\"") + std::string(arg0) + std::string("\""); 
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static std::string _OpenFileDialog(std::string arg0){
     std::string command;
     std::string result;

     command += "OpenFileDialog(";
     command += std::string("\"") + std::string(arg0) + std::string("\""); 
     command += ")";

     _execute_command(command, result);
     return result;
}


static std::string _FolderDialog(){
     std::string command;
     std::string result;

     command += "FolderDialog(";
     command += ")";

     _execute_command(command, result);
     return result;
}


static double _SelectAndLoadImageFile(double arg0,std::string arg1){
     std::string command;
     std::string result;

     command += "SelectAndLoadImageFile(";
     command += std::to_string(arg0);
     command += ",";
     command += std::string("\"") + std::string(arg1) + std::string("\""); 
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _SaveViewAsBitmap(double arg0,std::string arg1){
     std::string command;
     std::string result;

     command += "SaveViewAsBitmap(";
     command += std::to_string(arg0);
     command += ",";
     command += std::string("\"") + std::string(arg1) + std::string("\""); 
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _SaveArchivedImage(double arg0,std::string arg1){
     std::string command;
     std::string result;

     command += "SaveArchivedImage(";
     command += std::to_string(arg0);
     command += ",";
     command += std::string("\"") + std::string(arg1) + std::string("\""); 
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static std::string _GetDefaultPath(){
     std::string command;
     std::string result;

     command += "GetDefaultPath(";
     command += ")";

     _execute_command(command, result);
     return result;
}


static double _CheckFileExist(std::string arg0){
     std::string command;
     std::string result;

     command += "CheckFileExist(";
     command += std::string("\"") + std::string(arg0) + std::string("\""); 
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _EnumFiles(double arg0,std::string arg1,std::string arg2){
     std::string command;
     std::string result;

     command += "EnumFiles(";
     command += std::to_string(arg0);
     command += ",";
     command += std::string("\"") + std::string(arg1) + std::string("\""); 
     command += ",";
     command += std::string("\"") + std::string(arg2) + std::string("\""); 
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static std::string _GetNextFilePath(double arg0){
     std::string command;
     std::string result;

     command += "GetNextFilePath(";
     command += std::to_string(arg0);
     command += ")";

     _execute_command(command, result);
     return result;
}


static double _SelectAndSaveImageFile(double arg0,std::string arg1){
     std::string command;
     std::string result;

     command += "SelectAndSaveImageFile(";
     command += std::to_string(arg0);
     command += ",";
     command += std::string("\"") + std::string(arg1) + std::string("\""); 
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _LoadBitmap(double arg0,std::string arg1,double arg2,double arg3,double arg4,double arg5){
     std::string command;
     std::string result;

     command += "LoadBitmap(";
     command += std::to_string(arg0);
     command += ",";
     command += std::string("\"") + std::string(arg1) + std::string("\""); 
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _AddDataToChart(double arg0,double arg1){
     std::string command;
     std::string result;

     command += "AddDataToChart(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _AddDataToChart2(double arg0,double arg1,double arg2){
     std::string command;
     std::string result;

     command += "AddDataToChart2(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _ClearChart(double arg0){
     std::string command;
     std::string result;

     command += "ClearChart(";
     command += std::to_string(arg0);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _Res(double arg0){
     std::string command;
     std::string result;

     command += "Res(";
     command += std::to_string(arg0);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _ChangeRes(double arg0,double arg1){
     std::string command;
     std::string result;

     command += "ChangeRes(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static std::string _CreateDirectory(std::string arg0){
     std::string command;
     std::string result;

     command += "CreateDirectory(";
     command += std::string("\"") + std::string(arg0) + std::string("\""); 
     command += ")";

     _execute_command(command, result);
     return result;
}


static std::string _WriteText(std::string arg0,std::string arg1){
     std::string command;
     std::string result;

     command += "WriteText(";
     command += std::string("\"") + std::string(arg0) + std::string("\""); 
     command += ",";
     command += std::string("\"") + std::string(arg1) + std::string("\""); 
     command += ")";

     _execute_command(command, result);
     return result;
}


static std::string _AppendText(std::string arg0,std::string arg1){
     std::string command;
     std::string result;

     command += "AppendText(";
     command += std::string("\"") + std::string(arg0) + std::string("\""); 
     command += ",";
     command += std::string("\"") + std::string(arg1) + std::string("\""); 
     command += ")";

     _execute_command(command, result);
     return result;
}


static double _Wait(double arg0){
     std::string command;
     std::string result;

     command += "Wait(";
     command += std::to_string(arg0);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _StrLen(std::string arg0){
     std::string command;
     std::string result;

     command += "StrLen(";
     command += std::string("\"") + std::string(arg0) + std::string("\""); 
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static std::string _Substr(std::string arg0,double arg1,double arg2){
     std::string command;
     std::string result;

     command += "Substr(";
     command += std::string("\"") + std::string(arg0) + std::string("\""); 
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ")";

     _execute_command(command, result);
     return result;
}


static double _FindStr(std::string arg0,std::string arg1){
     std::string command;
     std::string result;

     command += "FindStr(";
     command += std::string("\"") + std::string(arg0) + std::string("\""); 
     command += ",";
     command += std::string("\"") + std::string(arg1) + std::string("\""); 
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static std::string _ReplaceStr(std::string arg0,std::string arg1,std::string arg2){
     std::string command;
     std::string result;

     command += "ReplaceStr(";
     command += std::string("\"") + std::string(arg0) + std::string("\""); 
     command += ",";
     command += std::string("\"") + std::string(arg1) + std::string("\""); 
     command += ",";
     command += std::string("\"") + std::string(arg2) + std::string("\""); 
     command += ")";

     _execute_command(command, result);
     return result;
}


static double _Linear2DataUnit(double arg0,double arg1,double arg2,double arg3,double arg4,double arg5,double arg6,double arg7){
     std::string command;
     std::string result;

     command += "Linear2DataUnit(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ",";
     command += std::to_string(arg6);
     command += ",";
     command += std::to_string(arg7);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _SumDataUnit(double arg0,double arg1,double arg2,double arg3,double arg4){
     std::string command;
     std::string result;

     command += "SumDataUnit(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _SubtractDataUnit(double arg0,double arg1,double arg2,double arg3,double arg4){
     std::string command;
     std::string result;

     command += "SubtractDataUnit(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _SubAbsDataUnit(double arg0,double arg1,double arg2,double arg3,double arg4){
     std::string command;
     std::string result;

     command += "SubAbsDataUnit(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _MultiplyDataUnit(double arg0,double arg1,double arg2,double arg3,double arg4){
     std::string command;
     std::string result;

     command += "MultiplyDataUnit(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _DivideDataUnit(double arg0,double arg1,double arg2,double arg3,double arg4){
     std::string command;
     std::string result;

     command += "DivideDataUnit(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _LinearDataUnit(double arg0,double arg1,double arg2,double arg3,double arg4,double arg5){
     std::string command;
     std::string result;

     command += "LinearDataUnit(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ",";
     command += std::to_string(arg2);
     command += ",";
     command += std::to_string(arg3);
     command += ",";
     command += std::to_string(arg4);
     command += ",";
     command += std::to_string(arg5);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


static double _AbsDataUnit(double arg0,double arg1){
     std::string command;
     std::string result;

     command += "AbsDataUnit(";
     command += std::to_string(arg0);
     command += ",";
     command += std::to_string(arg1);
     command += ")";

     _execute_command(command, result);
     return stod(result);
}


void pimpom_api_def(){
     p::def("I", _I);
     p::def("sin", _sin);
     p::def("cos", _cos);
     p::def("tan", _tan);
     p::def("log", _log);
     p::def("sqrt", _sqrt);
     p::def("exp", _exp);
     p::def("abs", _abs);
     p::def("random", _random);
     p::def("rand", _rand);
     p::def("ceil", _ceil);
     p::def("floor", _floor);
     p::def("asin", _asin);
     p::def("acos", _acos);
     p::def("pow", _pow);
     p::def("mod", _mod);
     p::def("atan2", _atan2);
     p::def("atan", _atan);
     p::def("gaus", _gaus);
     p::def("thresh", _thresh);
     p::def("delta", _delta);
     p::def("A", _A);
     p::def("M", _M);
     p::def("min", _min);
     p::def("max", _max);
     p::def("PI", _PI);
     p::def("dist", _dist);
     p::def("sigmoid", _sigmoid);
     p::def("ReLU", _ReLU);
     p::def("GetDataUnitNumber", _GetDataUnitNumber);
     p::def("GetCurrentImageNumber", _GetCurrentImageNumber);
     p::def("SetCurrentImageNumber", _SetCurrentImageNumber);
     p::def("GetDataTypeSize", _GetDataTypeSize);
     p::def("CopyDataUnit", _CopyDataUnit);
     p::def("DeleteDataUnit", _DeleteDataUnit);
     p::def("BackupDataUnit", _BackupDataUnit);
     p::def("RecoverDataUnit", _RecoverDataUnit);
     p::def("ConvertDataUnit", _ConvertDataUnit);
     p::def("ConvertDispToByte", _ConvertDispToByte);
     p::def("GetSrcFilePathName", _GetSrcFilePathName);
     p::def("GetSrcFilePath", _GetSrcFilePath);
     p::def("GetSrcFileName", _GetSrcFileName);
     p::def("GetSrcFileTitle", _GetSrcFileTitle);
     p::def("FormatDataUnit", _FormatDataUnit);
     p::def("GetDataMinMax", _GetDataMinMax);
     p::def("SetDispPage", _SetDispPage);
     p::def("SetDispChannel", _SetDispChannel);
     p::def("UseWorkArea", _UseWorkArea);
     p::def("DrawImage", _DrawImage);
     p::def("ScaleImage", _ScaleImage);
     p::def("SetDispRange", _SetDispRange);
     p::def("SetDispRangeAuto", _SetDispRangeAuto);
     p::def("SetWorkArea", _SetWorkArea);
     p::def("GetWorkAreaVtx", _GetWorkAreaVtx);
     p::def("GetWorkAreaOrnot", _GetWorkAreaOrnot);
     p::def("ChangePalette", _ChangePalette);
     p::def("AddMark", _AddMark);
     p::def("DeleteMark", _DeleteMark);
     p::def("GetMarkVal", _GetMarkVal);
     p::def("GetMarkNumber", _GetMarkNumber);
     p::def("ClearMark", _ClearMark);
     p::def("AddLineMark", _AddLineMark);
     p::def("ResizeDataUnit", _ResizeDataUnit);
     p::def("ClipDataUnit", _ClipDataUnit);
     p::def("PerspectiveTransDataUnit", _PerspectiveTransDataUnit);
     p::def("IsoTransDataUnit", _IsoTransDataUnit);
     p::def("CalcDataValue", _CalcDataValue);
     p::def("CalcMaskValue", _CalcMaskValue);
     p::def("FilterSmooth", _FilterSmooth);
     p::def("FilterGauss", _FilterGauss);
     p::def("FilterGaussUnshade", _FilterGaussUnshade);
     p::def("FilterMedian", _FilterMedian);
     p::def("FilterErosion", _FilterErosion);
     p::def("FilterDilation", _FilterDilation);
     p::def("FilterOpening", _FilterOpening);
     p::def("FilterClosing", _FilterClosing);
     p::def("FilterEdgeSobel", _FilterEdgeSobel);
     p::def("FilterLaplaceGauss", _FilterLaplaceGauss);
     p::def("FilterEdgeLog", _FilterEdgeLog);
     p::def("FilterMirror", _FilterMirror);
     p::def("FilterNega", _FilterNega);
     p::def("FilterEdgeCanny", _FilterEdgeCanny);
     p::def("FilterBilateral", _FilterBilateral);
     p::def("FilterGabor", _FilterGabor);
     p::def("FilterEdgeSigmoid", _FilterEdgeSigmoid);
     p::def("FilterCLAHE", _FilterCLAHE);
     p::def("FilterLocalContrast", _FilterLocalContrast);
     p::def("FilterLocalContrastPatch", _FilterLocalContrastPatch);
     p::def("Binalize", _Binalize);
     p::def("BinalizeThresholdOtsu", _BinalizeThresholdOtsu);
     p::def("BinalizeThresholdPTile", _BinalizeThresholdPTile);
     p::def("BinalizeLocalGauss", _BinalizeLocalGauss);
     p::def("BinalizeLocalLaplaceGauss", _BinalizeLocalLaplaceGauss);
     p::def("BinalizeNick", _BinalizeNick);
     p::def("Label", _Label);
     p::def("HoughLines", _HoughLines);
     p::def("BlendPoisson", _BlendPoisson);
     p::def("PrepeareDrawing", _PrepeareDrawing);
     p::def("DrawLineOnImage", _DrawLineOnImage);
     p::def("DrawCursorOnImage", _DrawCursorOnImage);
     p::def("DrawCircleOnImage", _DrawCircleOnImage);
     p::def("DrawRectOnImage", _DrawRectOnImage);
     p::def("DrawPointOnImage", _DrawPointOnImage);
     p::def("DrawTextOnImage", _DrawTextOnImage);
     p::def("FloodFillOnImage", _FloodFillOnImage);
     p::def("FillWorkAreaOnImage", _FillWorkAreaOnImage);
     p::def("DrawLineOnImageF", _DrawLineOnImageF);
     p::def("DrawImageOnImage", _DrawImageOnImage);
     p::def("FillWorkAreaOnMask", _FillWorkAreaOnMask);
     p::def("DrawLine", _DrawLine);
     p::def("DrawCursor", _DrawCursor);
     p::def("DrawRect", _DrawRect);
     p::def("DrawCircle", _DrawCircle);
     p::def("DrawEllipse", _DrawEllipse);
     p::def("DrawText", _DrawText);
     p::def("ClipSegment", _ClipSegment);
     p::def("ClipSegmentF", _ClipSegmentF);
     p::def("ShowMessage", _ShowMessage);
     p::def("MessageBox", _MessageBox);
     p::def("CopyToClipbord", _CopyToClipbord);
     p::def("PasteFromClipbord", _PasteFromClipbord);
     p::def("ToString", _ToString);
     p::def("ToNumber", _ToNumber);
     p::def("GetBatchInput", _GetBatchInput);
     p::def("SetBatchOutput", _SetBatchOutput);
     p::def("SetBatchInput", _SetBatchInput);
     p::def("GetBatchOutput", _GetBatchOutput);
     p::def("UserDlgBatchExecute", _UserDlgBatchExecute);
     p::def("CaptureOne", _CaptureOne);
     p::def("SendString", _SendString);
     p::def("OpenFileDialog", _OpenFileDialog);
     p::def("FolderDialog", _FolderDialog);
     p::def("SelectAndLoadImageFile", _SelectAndLoadImageFile);
     p::def("SaveViewAsBitmap", _SaveViewAsBitmap);
     p::def("SaveArchivedImage", _SaveArchivedImage);
     p::def("GetDefaultPath", _GetDefaultPath);
     p::def("CheckFileExist", _CheckFileExist);
     p::def("EnumFiles", _EnumFiles);
     p::def("GetNextFilePath", _GetNextFilePath);
     p::def("SelectAndSaveImageFile", _SelectAndSaveImageFile);
     p::def("LoadBitmap", _LoadBitmap);
     p::def("AddDataToChart", _AddDataToChart);
     p::def("AddDataToChart2", _AddDataToChart2);
     p::def("ClearChart", _ClearChart);
     p::def("Res", _Res);
     p::def("ChangeRes", _ChangeRes);
     p::def("CreateDirectory", _CreateDirectory);
     p::def("WriteText", _WriteText);
     p::def("AppendText", _AppendText);
     p::def("Wait", _Wait);
     p::def("StrLen", _StrLen);
     p::def("Substr", _Substr);
     p::def("FindStr", _FindStr);
     p::def("ReplaceStr", _ReplaceStr);
     p::def("Linear2DataUnit", _Linear2DataUnit);
     p::def("SumDataUnit", _SumDataUnit);
     p::def("SubtractDataUnit", _SubtractDataUnit);
     p::def("SubAbsDataUnit", _SubAbsDataUnit);
     p::def("MultiplyDataUnit", _MultiplyDataUnit);
     p::def("DivideDataUnit", _DivideDataUnit);
     p::def("LinearDataUnit", _LinearDataUnit);
     p::def("AbsDataUnit", _AbsDataUnit);
}
