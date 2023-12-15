#include "Headers/Buzz.h"
/*
// El constructor carga los modelos, más el usuario tiene que poner su shader
Buzz::Buzz()
{
	//Buzz right
	modelBuzzRightWing2.loadModel("../models/buzz/buzzlightyRightWing2.obj");
	modelBuzzRightWing1.loadModel("../models/buzz/buzzlightyRightWing1.obj");
	modelBuzzRightTigh.loadModel("../models/buzz/buzzlightyRightThigh.obj");
	modelBuzzRightHand.loadModel("../models/buzz/buzzlightyRightHand.obj");
	modelBuzzRightForearm.loadModel("../models/buzz/buzzlightyRightForearm.obj");
	modelBuzzRightFoot.loadModel("../models/buzz/buzzlightyRightFoot.obj");
	modelBuzzRightCalf.loadModel("../models/buzz/buzzlightyRightCalf.obj");
	modelBuzzRightArm.loadModel("../models/buzz/buzzlightyRightArm.obj");
	//Buzz left
	modelBuzzLeftWing2.loadModel("../models/buzz/buzzlightyLeftWing2.obj");
	modelBuzzLeftWing1.loadModel("../models/buzz/buzzlightyLeftWing1.obj");
	modelBuzzLeftTigh.loadModel("../models/buzz/buzzlightyLeftThigh.obj");
	modelBuzzLeftHand.loadModel("../models/buzz/buzzlightyLeftHand.obj");
	modelBuzzLeftForearm.loadModel("../models/buzz/buzzlightyLeftForearm.obj");
	modelBuzzLeftFoot.loadModel("../models/buzz/buzzlightyLeftFoot.obj");
	modelBuzzLeftCalf.loadModel("../models/buzz/buzzlightyLeftCalf.obj");
	modelBuzzLeftArm.loadModel("../models/buzz/buzzlightyLeftArm.obj");
	//Buzz middle
	modelBuzzTorso.loadModel("../models/buzz/buzzlightyTorso.obj");
	modelBuzzHip.loadModel("../models/buzz/buzzlightyHip.obj");
	modelBuzzHead.loadModel("../models/buzz/buzzlightyHead.obj");
};

Buzz::destroy(){
	//Buzz
	modelBuzzRightWing2.destroy();
	modelBuzzRightWing1.destroy();
	modelBuzzRightTigh.destroy();
	modelBuzzRightHand.destroy();
	modelBuzzRightForearm.destroy();
	modelBuzzRightFoot.destroy();
	modelBuzzRightCalf.destroy();
	modelBuzzRightArm.destroy();
	modelBuzzLeftWing2.destroy();
	modelBuzzLeftWing1.destroy();
	modelBuzzLeftTigh.destroy();
	modelBuzzLeftHand.destroy();
	modelBuzzLeftForearm.destroy();
	modelBuzzLeftFoot.destroy();
	modelBuzzLeftCalf.destroy();
	modelBuzzLeftArm.destroy();
	modelBuzzTorso.destroy();
	modelBuzzHip.destroy();
	modelBuzzHead.destroy();
}
*/
//La jerarquía debe de quedarse en el código principal o se puede pasar?
		/*
		Buzz::hierarchy(float rotBuzzBody, float rotBuzzHead, float rotBuzzLeftArm, float rotBuzzLeftForearm, 
						float rotBuzzLeftHand, float rotBuzzLeftThigh, float rotBuzzLeftCalf, float rotBuzzLeftFoot,
						float rotBuzzRightArm, float rotBuzzRightForearm, float rotBuzzRightHand, float rotBuzzRightThigh, 
						float rotBuzzRightCalf, float rotBuzzRightFoot){
			//Torso
			glm::mat4 modelMatrixBuzzBody = glm::mat4(modelMatrixBuzz);
			modelMatrixBuzzBody = glm::rotate(modelMatrixBuzzBody, rotBuzzBody, glm::vec3(0,1,0));
			modelBuzzTorso.render(modelMatrixBuzzBody);
			//Cabeza
			glm::mat4 modelMatrixBuzzHead = glm::mat4(modelMatrixBuzzBody);
			modelMatrixBuzzHead = rotate (modelMatrixBuzzHead, rotBuzzHead,glm::vec3(0,1,0));
			modelBuzzHead.render(modelMatrixBuzzHead);
			//Cadera
			glm::mat4 modelMatrixBuzzHip = glm::mat4(modelMatrixBuzzBody);
			modelBuzzHip.render(modelMatrixBuzzHip);
			//Ala izquierda 1 (la mas pegada al torso)
			glm::mat4 modelMatrixBuzzLeftWing1 = glm::mat4 (modelMatrixBuzzBody);
			modelBuzzLeftWing1.render(modelMatrixBuzzLeftWing1);
			//Ala izquierda 2
			glm::mat4 modelmatrixBuzzLeftWing2 = glm::mat4 (modelMatrixBuzzBody);
			modelBuzzLeftWing2.render(modelmatrixBuzzLeftWing2);
			//Ala derecha 1 (la mas pegada al torso)
			glm::mat4 modelMatrixBuzzRightWing1 = glm::mat4 (modelMatrixBuzzBody);
			modelBuzzRightWing1.render(modelMatrixBuzzRightWing1);
			//Ala derecha 2
			glm::mat4 modelmatrixBuzzRightWing2 = glm::mat4 (modelMatrixBuzzBody);
			modelBuzzRightWing2.render(modelmatrixBuzzRightWing2);

			//BUZZ POR LADOS:
			//Brazo izquierdo y su Pivote 
			glm::mat4 modelMatrixBuzzLeftArm = glm::mat4(modelMatrixBuzzBody);
			modelMatrixBuzzLeftArm = glm::translate(modelMatrixBuzzLeftArm,glm::vec3(0.178185,0.58335,-0.026702));
			modelMatrixBuzzLeftArm = glm::rotate(modelMatrixBuzzLeftArm, rotBuzzLeftArm, glm::vec3(1.0,0.0,0.0));
			modelMatrixBuzzLeftArm = glm::translate(modelMatrixBuzzLeftArm,glm::vec3(-0.178185,-0.58335,0.026702));
			modelBuzzLeftArm.render(modelMatrixBuzzLeftArm);
			//Antebrazo izquierdo y su pivote
			glm::mat4 modelMatrixBuzzLeftForearm = glm::mat4 (modelMatrixBuzzLeftArm);
			modelMatrixBuzzLeftForearm = glm::translate(modelMatrixBuzzLeftForearm,glm::vec3(0.3002,0.58,0.008279));
			modelMatrixBuzzLeftForearm = glm::rotate(modelMatrixBuzzLeftForearm, rotBuzzLeftForearm, glm::vec3(1.0,0.0,0.0));
			modelMatrixBuzzLeftForearm = glm::translate(modelMatrixBuzzLeftForearm,glm::vec3(-0.3002,-0.58,-0.008279));
			modelBuzzLeftForearm.render(modelMatrixBuzzLeftForearm);
			//Mano izquierda y su pivote
			glm::mat4 modelMatrixBuzzLeftHand = glm::mat4 (modelMatrixBuzzLeftForearm);
			modelMatrixBuzzLeftHand = glm::translate(modelMatrixBuzzLeftHand,glm::vec3(0.4055,0.5853,0.06706));
			modelMatrixBuzzLeftHand = glm::rotate(modelMatrixBuzzLeftHand, rotBuzzLeftHand, glm::vec3(1.0,0.0,0.0));
			modelMatrixBuzzLeftHand = glm::translate(modelMatrixBuzzLeftHand,glm::vec3(-0.4055,-0.5853,-0.06706));
			modelBuzzLeftHand.render(modelMatrixBuzzLeftHand);
			//Thigh izquierda
			glm::mat4 modelMatrixBuzzLeftThigh = glm::mat4 (modelMatrixBuzzBody);
			modelMatrixBuzzLeftThigh = glm::translate(modelMatrixBuzzLeftThigh,glm::vec3(0.05909,0.397,0.007883));
			modelMatrixBuzzLeftThigh = glm::rotate(modelMatrixBuzzLeftThigh, rotBuzzLeftThigh, glm::vec3(1.0,0.0,0.0));
			modelMatrixBuzzLeftThigh = glm::translate(modelMatrixBuzzLeftThigh,glm::vec3(-0.05909,-0.397,-0.007883));
			modelBuzzLeftTigh.render(modelMatrixBuzzLeftThigh);
			//Calf izquierdo
			glm::mat4 modelMatrixBuzzLeftCalf = glm::mat4 (modelMatrixBuzzLeftThigh);
			modelMatrixBuzzLeftCalf = glm::translate(modelMatrixBuzzLeftCalf,glm::vec3(0.065,0.2168,-0.007884));
			modelMatrixBuzzLeftCalf = glm::rotate(modelMatrixBuzzLeftCalf, rotBuzzLeftCalf, glm::vec3(1.0,0.0,0.0));
			modelMatrixBuzzLeftCalf = glm::translate(modelMatrixBuzzLeftCalf,glm::vec3(-0.065,-0.2168,0.007884));
			modelBuzzLeftCalf.render(modelMatrixBuzzLeftCalf);
			//Pie izquierdo
			glm::mat4 modelMatrixBuzzLeftFoot= glm::mat4 (modelMatrixBuzzLeftCalf);
			modelMatrixBuzzLeftFoot = glm::translate(modelMatrixBuzzLeftFoot,glm::vec3(0.72,0.08426,-0.02621));
			modelMatrixBuzzLeftFoot = glm::rotate(modelMatrixBuzzLeftFoot, rotBuzzLeftFoot, glm::vec3(1.0,0.0,0.0));
			modelMatrixBuzzLeftFoot = glm::translate(modelMatrixBuzzLeftFoot,glm::vec3(-0.72,-0.08426,0.02621));
			modelBuzzLeftFoot.render(modelMatrixBuzzLeftFoot);

			//RIGHT SIDE
			//Brazo derecho y su Pivote 
			glm::mat4 modelMatrixBuzzRightArm = glm::mat4(modelMatrixBuzzBody);
			modelMatrixBuzzRightArm = glm::translate(modelMatrixBuzzRightArm,glm::vec3(-0.178185,0.58335,-0.026702));
			modelMatrixBuzzRightArm = glm::rotate(modelMatrixBuzzRightArm, rotBuzzRightArm, glm::vec3(1.0,0.0,0.0));
			modelMatrixBuzzRightArm = glm::translate(modelMatrixBuzzRightArm,glm::vec3(0.178185,-0.58335,0.026702));
			modelBuzzRightArm.render(modelMatrixBuzzRightArm);
			//Antebrazo derecho y su pivote
			glm::mat4 modelMatrixBuzzRightForearm = glm::mat4 (modelMatrixBuzzRightArm);
			modelMatrixBuzzRightForearm = glm::translate(modelMatrixBuzzRightForearm,glm::vec3(-0.3002,0.58,0.008279));
			modelMatrixBuzzRightForearm = glm::rotate(modelMatrixBuzzRightForearm, rotBuzzRightForearm, glm::vec3(1.0,0.0,0.0));
			modelMatrixBuzzRightForearm = glm::translate(modelMatrixBuzzRightForearm,glm::vec3(0.3002,-0.58,-0.008279));
			modelBuzzRightForearm.render(modelMatrixBuzzRightForearm);
			//Mano derecha y su pivote
			glm::mat4 modelMatrixBuzzRightHand = glm::mat4 (modelMatrixBuzzRightForearm);
			modelMatrixBuzzRightHand = glm::translate(modelMatrixBuzzRightHand,glm::vec3(-0.4055,0.5853,0.06706));
			modelMatrixBuzzRightHand = glm::rotate(modelMatrixBuzzRightHand, rotBuzzRightHand, glm::vec3(1.0,0.0,0.0));
			modelMatrixBuzzRightHand = glm::translate(modelMatrixBuzzRightHand,glm::vec3(0.4055,-0.5853,-0.06706));
			modelBuzzRightHand.render(modelMatrixBuzzRightHand);
			//cchnagyy
			//Thigh derecha
			glm::mat4 modelMatrixBuzzRightThigh = glm::mat4 (modelMatrixBuzzBody);
			modelMatrixBuzzRightThigh = glm::translate(modelMatrixBuzzRightThigh,glm::vec3(-0.05909,0.397,0.007883));
			modelMatrixBuzzRightThigh = glm::rotate(modelMatrixBuzzRightThigh, rotBuzzRightThigh, glm::vec3(1.0,0.0,0.0));
			modelMatrixBuzzRightThigh = glm::translate(modelMatrixBuzzRightThigh,glm::vec3(0.05909,-0.397,-0.007883));
			modelBuzzRightTigh.render(modelMatrixBuzzRightThigh);
			//Calf derecha
			glm::mat4 modelMatrixBuzzRightCalf = glm::mat4 (modelMatrixBuzzRightThigh);
			modelMatrixBuzzRightCalf = glm::translate(modelMatrixBuzzRightCalf,glm::vec3(-0.065,0.2168,-0.007884));
			modelMatrixBuzzRightCalf = glm::rotate(modelMatrixBuzzRightCalf, rotBuzzRightCalf, glm::vec3(1.0,0.0,0.0));
			modelMatrixBuzzRightCalf = glm::translate(modelMatrixBuzzRightCalf,glm::vec3(0.065,-0.2168,0.007884));
			modelBuzzRightCalf.render(modelMatrixBuzzRightCalf);
			//Pie derecho
			glm::mat4 modelMatrixBuzzRightFoot= glm::mat4 (modelMatrixBuzzRightCalf);
			modelMatrixBuzzRightFoot = glm::translate(modelMatrixBuzzRightFoot,glm::vec3(-0.72,0.08426,-0.02621));
			modelMatrixBuzzRightFoot = glm::rotate(modelMatrixBuzzRightFoot, rotBuzzRightFoot, glm::vec3(1.0,0.0,0.0));
			modelMatrixBuzzRightFoot = glm::translate(modelMatrixBuzzRightFoot,glm::vec3(0.72,-0.08426,0.02621));
			modelBuzzRightFoot.render(modelMatrixBuzzRightFoot);
		}*/
/*
//Transformaciones a la matriz principal
Buzz::translate(glm::vec3 distance){ modelMatrixBuzz = glm::translate(modelMatrixBuzz, distance); }
Buzz::rotate(float angle, glm::vec3 axis){ modelMatrixBuzz = glm::rotate(modelMatrixBuzz, angle, axis); }
Buzz::scale(glm::vec3 scale){ modelMatrixBuzz = glm::scale(modelMatrixBuzz,scale); }
*/
Buzz::~Buzz() {
}
