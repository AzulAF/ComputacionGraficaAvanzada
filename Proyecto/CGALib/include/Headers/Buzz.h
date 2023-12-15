
#ifndef BUZZ_H
#define BUZZ_H

#if defined _WIN32 || defined __CYGWIN__
  #ifdef BUILDING_DLL
    #ifdef __GNUC__
      #define DLL_PUBLIC __attribute__ ((dllexport))
    #else
      #define DLL_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #else
    #ifdef __GNUC__
      #define DLL_PUBLIC __attribute__ ((dllimport))
    #else
      #define DLL_PUBLIC __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #endif
  #define DLL_LOCAL
#else
  #if __GNUC__ >= 4
    #define DLL_PUBLIC __attribute__ ((visibility ("default")))
    #define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
    #define DLL_PUBLIC
    #define DLL_LOCAL
  #endif
#endif

#include "AbstractModel.h"
#include "Model.h"

class DLL_PUBLIC Buzz : public AbstractModel
{
public:
    Buzz();
    destroy();
    translate(glm::vec3 distance);
    rotate(float angle, glm::vec3 axis);
    scale(glm::vec3 scale);
	~Buzz();
	//Buzz right
	Model modelBuzzRightWing2;
	Model modelBuzzRightWing1;
	Model modelBuzzRightTigh;
	Model modelBuzzRightHand;
	Model modelBuzzRightForearm;
	Model modelBuzzRightFoot;
	Model modelBuzzRightCalf;
	Model modelBuzzRightArm;
	//Buzz left
	Model modelBuzzLeftWing2;
	Model modelBuzzLeftWing1;
	Model modelBuzzLeftTigh;
	Model modelBuzzLeftHand;
	Model modelBuzzLeftForearm;
	Model modelBuzzLeftFoot;
	Model modelBuzzLeftCalf;
	Model modelBuzzLeftArm;
	//Buzz middle
	Model modelBuzzTorso;
	Model modelBuzzHip;
	Model modelBuzzHead;
	//Matriz principal
	glm::mat4 modelMatrixBuzz = glm::mat4(1.0f);
};

#endif // BUZZ_H
