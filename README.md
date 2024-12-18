This is not a fully functional program.  It is a placeholder to work with and develop libraries 
  based on Ollama.

  References and sources are scattered throughout the code.
  
  All apologies for anyone or anybody who was looking for a program well defined and documented.

--- 

Compile Notes:
  "ollama-hpp" should be in the same directory as "ollama_chat"

To Compile:
  "cd" into build.
  Run "cmak.sh" to build Cmake Files (unnecessary?) (Run Once)
  Run "m.sh" subsequencially for any changes to build the executable.

---

Dependancies Notes:

sudo apt-get update

sudo apt-get install nlohmann-json3-dev
sudo apt-get install libssl-dev
sudo apt install libcurl4-openssl-dev
sudo apt install libboost-filesystem-dev


cpp-httplib
gh repo clone yhirose/cpp-httplib

https://github.com/ollama/ollama

https://github.com/jmont-dev/ollama-hpp
gh repo clone jmont-dev/ollama-hpp

https://github.com/BlackyDrum/chromadb-cpp
gh repo clone BlackyDrum/chromadb-cpp

---

ChromaDB Notes:

https://docs.trychroma.com/deployment/auth#static-api-token-authentication

---

BuOllama Build / Install, things to do. (https://github.com/ollama/ollama/blob/main/docs/development.md)

CUDA Toolkit Installer: (not sure it works correctly) (https://developer.nvidia.com/cuda-downloads?target_os=Linux&target_arch=x86_64&Distribution=WSL-Ubuntu&target_version=2.0&target_type=runfile_local)
wget https://developer.download.nvidia.com/compute/cuda/12.6.2/local_installers/cuda_12.6.2_560.35.03_linux.run
sudo sh cuda_12.6.2_560.35.03_linux.run

verify -lcuda installed correctly.  (If error, gives you pointer to the correct spot.)
nvcc --version


Python Notes:

sudo apt install python3-pip

sudo apt install python3.12-venv
python3 -m venv ./venv
source venv/bin/activate
pip install chromadb

pip list --outdated
pip3 install --upgrade package

pip3 install nltk

pip install scikit-learn
https://scikit-learn.org/stable/modules/generated/sklearn.metrics.pairwise_distances.html?form=MG0AV3


export OLLAMA_NUM_PARALLEL=3
export OLLAMA_MAX_LOADED_MODELS=3

---

Model manager
Page doesnt load on my system 
https://smcleod.net/2024/06/gollama-ollama-model-manager/