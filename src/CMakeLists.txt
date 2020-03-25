#LIBS
set(PROJECT_LIBS 
    ${CMAKE_THREAD_LIBS_INIT}
    rt.a
    m.a
    z.a
    )

#APP
add_executable(${PROJECT_NAME} main.cpp Actors/Actor.cpp Actors/Actor.h Actors/ActorThread.cpp Actors/ActorThread.h Actors/Callback.h ABActor.cpp ABActor.h WokrerActor.cpp WokrerActor.h Coroutine/ActorAwaiter.cpp Coroutine/ActorAwaiter.h Coroutine/ActorResumable.cpp Coroutine/ActorResumable.h Coroutine/CoroTask.cpp Coroutine/CoroTask.h)
target_link_libraries(${PROJECT_NAME} ${PROJECT_LIBS})
