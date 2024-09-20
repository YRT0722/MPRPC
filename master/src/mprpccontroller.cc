#include"mprpccontroller.h"

 MprpcController::MprpcController()
 {
    m_failed=false;
    m_errText="";
 }
void MprpcController::Reset()
{
    m_failed=false;
    m_errText="";
}
bool MprpcController::Failed() const
{
    return m_failed;
}
  // If Failed() is true, returns a human-readable description of the error.
std::string MprpcController::ErrorText() const
{
    //m_failed=true;
    return m_errText;
}

void MprpcController::SetFailed(const std::string& reason) 
{
    m_failed=true;
    m_errText=reason;
}


bool MprpcController::IsCanceled() const
{
    return true;
}
void MprpcController::NotifyOnCancel(google::protobuf::Closure* callback)
{

}
void MprpcController::StartCancel()
{

}