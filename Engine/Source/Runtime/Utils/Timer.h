#pragma once
#include <unordered_map>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cuda_runtime.h>

#include "Core/Log.h"

class Timer {
public:
    Timer() {
        s_Timer = this;

        m_LastUpdateTime = static_cast<float>(CurrentTime());
        m_FixedUpdateTimer = static_cast<float>(CurrentTime());
    }

    ~Timer() {
        for (const auto &label2events: m_CudaEvents) {
            for (auto &e: label2events.second) {
                cudaEventDestroy(e);
            }
        }
    }

    static void StartTimer(const std::string &label) {
        s_Timer->m_Times[label] = CurrentTime();
    }

    // Returns elapsed time
    // When called multiple time during one frame, result gets accumulated.
    static double EndTimer(const std::string &label, int frame = -1) {
        const double time = CurrentTime() - s_Timer->m_Times[label];
        if (frame == -1) {
            frame = s_Timer->m_FrameCount;
        }

        if (s_Timer->m_Times.count(label)) {
            if (frame > s_Timer->m_Frames[label]) {
                s_Timer->m_History[label] = time;
            } else {
                s_Timer->m_History[label] += time;
            }
            s_Timer->m_Frames[label] = frame;
            return s_Timer->m_History[label];
        }
        LOG_WARN("Warning(Timer): EndTimer with undefined label[{}].\n", label);
        return -1;
    }

    static double GetTimer(const std::string &label) {
        if (s_Timer->m_History.count(label)) {
            return s_Timer->m_History[label];
        }
        return 0;
    }

    static double CurrentTime() {
        return glfwGetTime();
    }

public:
    static void StartTimerGPU(const std::string &label) {
        const int frame = s_Timer->m_FrameCount;

        if (s_Timer->m_Frames.count(label) && s_Timer->m_Frames[label] != frame) {
            GetTimerGPU(label);
        }
        s_Timer->m_Frames[label] = frame;

        cudaEvent_t start, end;
        cudaEventCreate(&start);
        cudaEventCreate(&end);
        auto &events = s_Timer->m_CudaEvents[label];
        events.push_back(start);
        events.push_back(end);
        cudaEventRecord(start);
    }

    static void EndTimerGPU(const std::string &label) {
        const auto &events = s_Timer->m_CudaEvents[label];
        auto stop = events[events.size() - 1];
        cudaEventRecord(stop);
    }

    // return time in mili seconds
    static double GetTimerGPU(const std::string &label) {
        auto &events = s_Timer->m_CudaEvents[label];
        if (!events.empty()) {
            const auto lastEvent = events[events.size() - 1];
            cudaEventSynchronize(lastEvent);

            float totalTime = 0.0f;
            for (int i = 0; i < events.size(); i += 2) {
                float time;
                cudaEventElapsedTime(&time, events[i], events[i + 1]);
                totalTime += time;
                cudaEventDestroy(events[i]);
                cudaEventDestroy(events[i + 1]);
            }
            events.clear();
            s_Timer->m_History[label] = totalTime;
        }

        if (s_Timer->m_History.count(label)) {
            return s_Timer->m_History[label];
        }
        return 0;
    }

public:
    static void UpdateDeltaTime() {
        auto current = static_cast<float>(CurrentTime());
        s_Timer->m_DeltaTime = std::min(current - s_Timer->m_LastUpdateTime, 0.2f);
        s_Timer->m_LastUpdateTime = current;

        LOG_INFO("dt: {}", s_Timer->m_DeltaTime);
    }

    static void NextFrame() {
        s_Timer->m_FrameCount++;
        s_Timer->m_ElapsedTime += s_Timer->m_DeltaTime;
    }

    // Return true when fixed update should be executed
    static bool NextFixedFrame() {
        s_Timer->m_FixedUpdateTimer += s_Timer->m_DeltaTime;

        if (s_Timer->m_FixedUpdateTimer > s_Timer->m_FixedDeltaTime) {
            s_Timer->m_FixedUpdateTimer = 0;
            s_Timer->m_PhysicsFrameCount++;
            return true;
        }
        return false;
    }

    static bool PeriodicUpdate(const std::string &label, float interval, bool allowRepetition = true) {
        auto &l2t = s_Timer->m_Label2AccumulatedTime;
        if (!l2t.count(label)) {
            l2t[label] = 0;
        }

        if (l2t[label] < s_Timer->m_ElapsedTime) {
            l2t[label] = allowRepetition ? l2t[label] + interval : s_Timer->m_ElapsedTime + interval;
            return true;
        }
        return false;
    }

    static auto GetFrameCount() {
        return s_Timer->m_FrameCount;
    }

    static auto GetPhysicsFrameCount() {
        return s_Timer->m_PhysicsFrameCount;
    }

    static auto GetElapsedTime() {
        return s_Timer->m_ElapsedTime;
    }

    static auto GetDeltaTime() {
        return s_Timer->m_DeltaTime;
    }

    static auto GetFixedDeltaTime() {
        return s_Timer->m_FixedDeltaTime;
    }

private:
    static Timer *s_Timer;

    std::unordered_map<std::string, double> m_Times;
    std::unordered_map<std::string, double> m_History;
    std::unordered_map<std::string, int> m_Frames;
    std::unordered_map<std::string, std::vector<cudaEvent_t> > m_CudaEvents;
    std::unordered_map<std::string, float> m_Label2AccumulatedTime;

    int m_FrameCount = 0;
    int m_PhysicsFrameCount = 0;
    float m_ElapsedTime = 0.0f;
    float m_DeltaTime = 0.0f;
    const float m_FixedDeltaTime = 1.0f / 60.0f;

    float m_LastUpdateTime = 0.0f;
    float m_FixedUpdateTimer = 0.0f;
};

class ScopedTimerGPU {
public:
    explicit ScopedTimerGPU(const std::string &&label) {
        m_Label = label;
        Timer::StartTimerGPU(label);
    }

    ~ScopedTimerGPU() {
        Timer::EndTimerGPU(m_Label);
    }

private:
    std::string m_Label;
};
