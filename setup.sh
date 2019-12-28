function env_set() {
    sed -i -e "/^export $2=.*/d" ~/.bashrc
    echo "export $2=`readlink -e $1`" >> ~/.bashrc
}

[ -z ${STUID} ]   && echo "STUID must be set (RTFM)"   && exit
[ -z ${STUNAME} ] && echo "STUNAME must be set (RTFM)" && exit
echo "uploading public key..."
# curl -F "id=${STUID}" -F "name=${STUNAME}" -F "file=@${HOME}/.ssh/id_rsa.pub" https://dssl.cun.io/teach/api/uploads/pasetup

curl -F "id=${STUID}" -F "name=${STUNAME}" -F "file=@${HOME}/.ssh/id_rsa.pub" https://cernet.cnm.cool/teach/api/uploads/pasetup # reverse proxy

sleep 2
echo "setup git-remote..."
# git remote add hustpa pa@dssl.cun.io:${STUID}-git

git remote add hustpa pa@cernet.cnm.cool:${STUID}-git # reverse proxy

git branch pa0
git branch pa1
git branch pa2
git branch pa3
git branch pa4
git branch pa5
git push hustpa pa0
git push hustpa pa1
git push hustpa pa2
git push hustpa pa3
git push hustpa pa4
git push hustpa pa5
git branch -u hustpa/pa0 pa0
git branch -u hustpa/pa1 pa1
git branch -u hustpa/pa2 pa2
git branch -u hustpa/pa3 pa3
git branch -u hustpa/pa4 pa4
git branch -u hustpa/pa5 pa5
echo "set ENV ..."
env_set nemu NEMU_HOME
env_set nexus-am AM_HOME
env_set navy-apps NAVY_HOME
source ~/.bashrc
echo "done!"
