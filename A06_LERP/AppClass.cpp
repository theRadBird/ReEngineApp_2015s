#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Assignment  06 - LERP"); // Window Name
}

void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 0.0f, 15.0f), ZERO_V3, REAXISY);

	// Color of the screen
	m_v4ClearColor = vector4(REBLACK, 1); // Set the clear color to black

	m_pMeshMngr->LoadModel("Sorted\\WallEye.bto", "WallEye");

	fDuration = 1.0f;

    //Add all character positions
    m_locations.push_back(vector3(-4.0f, -2.0f, 5.0f));
    m_locations.push_back(vector3(1.0f, -2.0f, 5.0f));
    m_locations.push_back(vector3(-3.0f, -1.0f, 3.0f));
    m_locations.push_back(vector3(2.0f, -1.0f, 3.0f));
    m_locations.push_back(vector3(-2.0f, 0.0f, 0.0f));
    m_locations.push_back(vector3(3.0f, 0.0f, 0.0f));
    m_locations.push_back(vector3(-1.0f, 1.0f, -3.0f));
    m_locations.push_back(vector3(4.0f, 1.0f, -3.0f));
    m_locations.push_back(vector3(0.0f, 2.0f, -5.0f));
    m_locations.push_back(vector3(5.0f, 2.0f, -5.0f));
    m_locations.push_back(vector3(1.0f, 3.0f, -5.0f));
    
    m_dataSize = m_locations.size();

    //Make Sphere
    m_pSphere = new PrimitiveClass();
    m_pMatrix = new matrix4[m_dataSize];

    m_pSphere->GenerateSphere(0.1f, 8, RERED);
    m_pSphere = new PrimitiveClass[m_dataSize];

    for (uint i = 0; i < m_dataSize; i++)
    {
        m_pSphere[i].GenerateSphere(0.5f, 8, RERED);
    }
}

void AppClass::Update(void)
{
#pragma region Does not change anything here
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();
#pragma region

#pragma region Does not need changes but feel free to change anything here
	//Lets us know how much time has passed since the last call
	double fTimeSpan = m_pSystem->LapClock(); //Delta time (between frame calls)

	//cumulative time
	static double fRunTime = 0.0f; //How much time has passed since the program started
	fRunTime += fTimeSpan; 
#pragma endregion

#pragma region Your Code goes here
	m_pMeshMngr->SetModelMatrix(IDENTITY_M4, "WallEye");
	
	//loop through all locations, move from point to point.
	for (uint i = 0; i < m_dataSize; i++)
	{
        float fPercent = MapValue(
            static_cast<float>(i),
            0.0f,
            static_cast<float>(m_dataSize),
            0.0f,
            1.0f
            );

        //If on the last point, we want to make sure to go to the 1st position.
        //this doesn't actually move... anything...
        if ( i != m_dataSize) 
        {
            //this needs to take one i position and move it to the next but it doesn't so RIP
            vector3 v3Start(static_cast<float>(m_dataSize), 0.0f, 0.0f);
            vector3 v3End = glm::lerp(v3Start, v3End, static_cast<float>(1));

            vector3 v3Current = glm::lerp(v3Start, v3End, fPercent);
            m_pMatrix[i] = glm::translate(v3Current);
        }
        else 
        {
            vector3 v3Start(static_cast<float>(m_dataSize), 0.0f, 0.0f);
            vector3 v3End = glm::lerp(v3Start, v3End, static_cast<float>(1));

            vector3 v3Current = glm::lerp(v3Start, v3End, fPercent);
            m_pMatrix[i] = glm::translate(v3Current);
        }
	}

#pragma endregion

#pragma region Does not need changes but feel free to change anything here
	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
#pragma endregion
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();

	//Render the grid based on the camera's mode:
	switch (m_pCameraMngr->GetCameraMode())
	{
	default: //Perspective
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XY); //renders the XY grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOX:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::YZ, RERED * 0.75f); //renders the YZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOY:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XZ, REGREEN * 0.75f); //renders the XZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOZ:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XY, REBLUE * 0.75f); //renders the XY grid with a 100% scale
		break;
	}
	
    matrix4 mProjection = m_pCameraMngr->GetProjectionMatrix();
    matrix4 mView = m_pCameraMngr->GetViewMatrix();

    //Makes spheres in different locations - JUST KIDDING nothing shows up :'(
    for (uint i = 0; i < m_dataSize; i++)
    {
        m_pMatrix[i] = glm::translate(m_locations[i]);
        m_pSphere[i].Render(mProjection, mView, m_pMatrix[i]);
    }

	m_pMeshMngr->Render(); //renders the render list

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields

    if (m_pSphere != nullptr)
    {
        delete[] m_pSphere;
        m_pSphere = nullptr;
    }

    if (m_pMatrix != nullptr)
    {
        delete[] m_pMatrix;
        m_pMatrix = nullptr;
    }
}