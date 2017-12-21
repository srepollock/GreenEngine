#pragma once
#ifdef __APPLE__
#elif defined _WIN32 || defined _WIN64
#endif
class Engine {
public:
    Engine();
    ~Engine();
    void Setup();
    void Start();
    void Stop();
    void Loop();
    bool GetEngineRunning();
private:
    bool engineRunning = false; // Defaults to no
    void SetEngineRunning(bool);
    void UpdateObjects();
};
