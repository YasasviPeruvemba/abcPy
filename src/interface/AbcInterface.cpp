#include "AbcInterface.h"


#if defined(ABC_NAMESPACE)
namespace ABC_NAMESPACE
{
#elif defined(__cplusplus)
extern "C"
{
#endif

// procedures to start and stop the ABC framework
// (should be called before and after the ABC procedures are called)
void   Abc_Start();
void   Abc_Stop();

// procedures to get the ABC framework and execute commands in it
//typedef struct Abc_Frame_t_ Abc_Frame_t;

Abc_Frame_t_ * Abc_FrameGetGlobalFrame();
int    Cmd_CommandExecute( Abc_Frame_t_ * pAbc, const char * sCommand );


#if defined(ABC_NAMESPACE)
}
using namespace ABC_NAMESPACE;
#elif defined(__cplusplus)
}
#endif

PROJECT_NAMESPACE_BEGIN


/// @brief strip the dash after the string and return the integer the string represent
IntType stripDash(std::string word)
{
    auto pos = word.rfind('/');
    while (pos != std::string::npos)
    {
        word = word.substr(0, pos);
        pos = word.rfind('/');
    }
    return ::atoi(word.c_str());
}

/// @brief strip the dash after the string and return the integer the string represent
IntType stripEqual(std::string word)
{
    auto pos = word.find('=');
    while (pos != std::string::npos)
    {
        word = word.substr(pos+1);
        pos = word.find('=');
    }
    return ::atoi(word.c_str());
}

void AbcInterface::start()
{
    // start the ABC framework
    Abc_Start();
    _pAbc = Abc_FrameGetGlobalFrame();
    // Std streams capture
    //_stdCap.Init();
}

void AbcInterface::end()
{
    // stop t;he ABC framework
    Abc_Stop();
}

float AbcInterface::read(const std::string &filename)
{
    auto beginClk = clock();
    char Command[1000];
    // read the file
    sprintf( Command, "read %s", filename.c_str() );
    if ( Cmd_CommandExecute( _pAbc, Command ) )
    {
        ERR("Cannot execute command \"%s\".\n", Command );
        return false;
    }
    // Default do a strash
    sprintf( Command, "strash" );
    if ( Cmd_CommandExecute( _pAbc, Command ) )
    {
        ERR("Cannot execute command \"%s\".\n", Command );
        return -1.0;
    }
    auto endClk = clock();
    _lastClk = endClk - beginClk;
    this->updateGraph();
    return (float)_lastClk/CLOCKS_PER_SEC;
}

float AbcInterface::balance(bool l, bool d, bool s, bool x)
{
    std::string cmd = "balance";
    if (l)
    {
        cmd += " -l ";
    }
    if (d)
    {
        cmd += " -d ";
    }
    if (s)
    {
        cmd += " -s ";
    }
    if (x)
    {
        cmd += " -x ";
    }
    auto beginClk = clock();
    if ( Cmd_CommandExecute( _pAbc, cmd.c_str() ) )
    {
        ERR("Cannot execute command \"%s\".\n", cmd.c_str() );
        return -1.0;
    }
    auto endClk = clock();
    _lastClk = endClk - beginClk;
    return (float)_lastClk/CLOCKS_PER_SEC;
}

float AbcInterface::resub(IntType k, IntType n, IntType f, bool l, bool z)
{
    std::string cmd = "resub";
    if (k != -1)
    {
        cmd += " -K ";
        cmd += std::to_string(k);
    }
    if (n != -1)
    {
        cmd += " -N ";
        cmd += std::to_string(n);
    }
    if (f != -1)
    {
        cmd += " -F ";
        cmd += std::to_string(f);
    }
    if (l)
    {
        cmd += " -l ";
    }
    if (z)
    {
        cmd += " -z ";
    }
    auto beginClk = clock();
    if ( Cmd_CommandExecute( _pAbc, cmd.c_str() ) )
    {
        ERR("Cannot execute command \"%s\".\n", cmd.c_str() );
        return -1.0;
    }
    auto endClk = clock();
    _lastClk = endClk - beginClk;
    return (float)_lastClk/CLOCKS_PER_SEC;
}

float AbcInterface::rewrite(bool l, bool z)
{
    std::string cmd = "rewrite";
    if (l)
    {
        cmd += " -l ";
    }
    if (z)
    {
        cmd += " -z ";
    }
    auto beginClk = clock();
    if ( Cmd_CommandExecute( _pAbc, cmd.c_str() ) )
    {
        ERR("Cannot execute command \"%s\".\n", cmd.c_str() );
        return -1.0;
    }
    auto endClk = clock();
    _lastClk = endClk - beginClk;
    return (float)_lastClk/CLOCKS_PER_SEC;
}

float AbcInterface::refactor(IntType n, bool l, bool z)
{
    std::string cmd = "refactor";
    if (n != -1)
    {
        cmd += " -N ";
        cmd += std::to_string(n);
    }
    if (l)
    {
        cmd += " -l ";
    }
    if (z)
    {
        cmd += " -z ";
    }
    auto beginClk = clock();
    if ( Cmd_CommandExecute( _pAbc, cmd.c_str() ) )
    {
        ERR("Cannot execute command \"%s\".\n", cmd.c_str() );
        return -1.0;
    }
    auto endClk = clock();
    _lastClk = endClk - beginClk;
    return (float)_lastClk/CLOCKS_PER_SEC;
}

float AbcInterface::compress2rs()
{
    // "b -l; rs -K 6 -l; rw -l; rs -K 6 -N 2 -l; rf -l; rs -K 8 -l; b -l; rs -K 8 -N 2 -l; rw -l; rs -K 10 -l; rwz -l; rs -K 10 -N 2 -l; b -l; rs -K 12 -l; rfz -l; rs -K 12 -N 2 -l; rwz -l; b -l
    auto beginClk = clock();
    if (this->balance(true) == -1.0) { return -1.0; }
    if (this->resub(6, -1, -1, true, false) == -1.0) { return -1.0; }
    if (this->rewrite(true, false) == -1.0) { return -1.0; }
    if (this->resub(6, 2, -1, true, false) == -1.0) { return -1.0; }
    if (this->refactor(-1, true, false) == -1.0) { return -1.0; }
    if (this->resub(8, -1, -1, true, false) == -1.0) { return -1.0; }
    if (this->balance(true, false, false,false) == -1.0) { return -1.0; }
    if (this->resub(8, 2, -1, true, false) == -1.0) { return -1.0; }
    if (this->rewrite(true, false) == -1.0) { return -1.0; }
    if (this->resub(10, -1, -1, true, false) == -1.0) { return -1.0; }
    if (this->rewrite(true, true) == -1.0) { return -1.0; }
    if (this->resub(10, 2, -1, true, false) == -1.0) { return -1.0; }
    if (this->balance(true, false, false, false) == -1.0) { return -1.0; }
    if (this->resub(12, -1, -1, true, false) == -1.0) { return -1.0; }
    if (this->refactor(-1, true, true) == -1.0) { return -1.0; }
    if (this->resub(12, 2, -1, true, false) == -1.0) { return -1.0; }
    if (this->rewrite(true, true) == -1.0) { return -1.0; }
    if (this->balance(true, false, false, false) == -1.0) { return -1.0; }
    auto endClk = clock();
    _lastClk = endClk - beginClk;
    return (float)_lastClk/CLOCKS_PER_SEC;
}

float AbcInterface::resyn2rs()
{
    // "b; rs -K 6; rw; rs -K 6 -N 2; rf; rs -K 8; b; rs -K 8 -N 2; rw; rs -K 10; rwz; rs -K 10 -N 2; b; rs -K 12; rfz; rs     -K 12 -N 2; rwz; b"
    auto beginClk = clock();
    if (this->balance() == -1.0) { return -1.0; }
    if (this->resub(6, -1, -1, false, false) == -1.0) { return -1.0; }
    if (this->rewrite(false, false) == -1.0) { return -1.0; }
    if (this->resub(6, 2, -1, false, false) == -1.0) { return -1.0; }
    if (this->refactor(-1, false, false) == -1.0) { return -1.0; }
    if (this->resub(8, -1, -1, false, false) == -1.0) { return -1.0; }
    if (this->balance(false, false, false,false) == -1.0) { return -1.0; }
    if (this->resub(8, 2, -1, false, false) == -1.0) { return -1.0; }
    if (this->rewrite(false, false) == -1.0) { return -1.0; }
    if (this->resub(10, -1, -1, false, false) == -1.0) { return -1.0; }
    if (this->rewrite(false, true) == -1.0) { return -1.0; }
    if (this->resub(10, 2, -1, false, false) == -1.0) { return -1.0; }
    if (this->balance(false, false, false, false) == -1.0) { return -1.0; }
    if (this->resub(12, -1, -1, false, false) == -1.0) { return -1.0; }
    if (this->refactor(-1, false, true) == -1.0) { return -1.0; }
    if (this->resub(12, 2, -1, false, false) == -1.0) { return -1.0; }
    if (this->rewrite(false, true) == -1.0) { return -1.0; }
    if (this->balance(false, false, false, false) == -1.0) { return -1.0; }
    auto endClk = clock();
    _lastClk = endClk - beginClk;
    return (float)_lastClk/CLOCKS_PER_SEC;
}

float AbcInterface::dch()
{
    std::string cmd = "dch";
    auto beginClk = clock();
    if ( Cmd_CommandExecute( _pAbc, cmd.c_str() ) )
    {
        ERR("Cannot execute command \"%s\".\n", cmd.c_str() );
        return -1.0;
    }
    auto endClk = clock();
    _lastClk = endClk - beginClk;
    return (float)_lastClk/CLOCKS_PER_SEC;
}

float AbcInterface::dc2()
{
    std::string cmd = "dc2";
    auto beginClk = clock();
    if ( Cmd_CommandExecute( _pAbc, cmd.c_str() ) )
    {
        ERR("Cannot execute command \"%s\".\n", cmd.c_str() );
        return -1.0;
    }
    auto endClk = clock();
    _lastClk = endClk - beginClk;
    return (float)_lastClk/CLOCKS_PER_SEC;
}

IntType AbcInterface::numNodes()
{
    IntType nObj = _pAbc->pNtkCur->nObjs;
    return nObj;
}

void AbcInterface::updateGraph()
{
    _numAigAnds = 0;
    _depth = -1;
    _numPO = 0;
    _numPI = 0;
    _numConst = 0;

    _aigNodes.resize(this->numNodes());
    //DBG("update graph: num of nodes %d \n", this->numNodes());

    for (IntType idx = 0; idx < this->numNodes(); ++idx)
    {
        auto pObj = (Abc_Obj_t*)_pAbc->pNtkCur->vObjs->pArray[idx];
        if (pObj->Level > _depth)
        {
            _depth = pObj->Level;
        }
        if (pObj->Type == ABC_OBJ_CONST1)
        {
            _numConst++;
        }
        else if (pObj->Type == ABC_OBJ_PI)
        {
            _numPI++;
        }
        else if (pObj->Type == ABC_OBJ_PO)
        {
            _numPO++;
        }
        else if (pObj->Type == ABC_OBJ_NODE)
        {
            _numAigAnds++;
        }
        else
        {
            AssertMsg(false, "Unexpected node type %d \n", pObj->Type);
        }
        // Configure the AIG node maintained
        _aigNodes[idx].configureNodeFromAbc(pObj);
    }
}

AigStats AbcInterface::aigStats()
{
    this->updateGraph();
    AigStats stats;
    stats.setNumIn(_numPI);
    stats.setNumOut(_numPO);
    stats.setNumAnd(_numAigAnds);
    stats.setLev(_depth);
    return stats;
}

PROJECT_NAMESPACE_END
