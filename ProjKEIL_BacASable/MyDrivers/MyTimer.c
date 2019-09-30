// TOUT A FAIRE !! //

/*
 indispensable pour pouvoir adresser les registres des périphériques.
 Rem : OBLIGATION d'utiliser les définitions utiles contenues dans ce fichier (ex : TIM_CR1_CEN, RCC_APB1ENR_TIM2EN ...)
 pour une meilleure lisibilité du code.

 Pour les masques, utiliser également les définitions proposée
 Rappel : pour mettre à 1  , reg = reg | Mask (ou Mask est le représente le ou les bits à positionner à 1)
				  pour mettre à 0  , reg = reg&~ Mask (ou Mask est le représente le ou les bits à positionner à 0)
 
*/ 
#include "stm32f103xb.h" 
//activer horloge
//ARR = autoreload = nb de step - 1
//PSC = prescaler
//stocker nom de la fonction en private : voir pointeurs de fonction
/**
	* @brief  Active l'horloge et règle l'ARR et le PSC du timer visé
  * @note   Fonction à lancer avant toute autre. Le timer n'est pas encore lancé (voir MyTimerStart)
	* @param  TIM_TypeDef Timer : indique le timer à utiliser par le chronomètre, TIM1, TIM2, TIM3 ou TIM4
	* 				int Arr   : valeur à placer dans ARR
	*					int Psc   : valeur à placer dans PSC
  * @retval None
  */
void MyTimer_Conf(TIM_TypeDef * Timer,int Arr, int Psc) {
	if (Timer == TIM1)	
		RCC -> APB2ENR |= RCC_APB2ENR_TIM1EN;
	else if (Timer == TIM2)
		RCC -> APB1ENR |= RCC_APB1ENR_TIM2EN;
	else if (Timer == TIM3)
		RCC -> APB1ENR |= RCC_APB1ENR_TIM3EN;
	else if (Timer == TIM4)
		RCC -> APB1ENR |= RCC_APB1ENR_TIM4EN;
	
	Timer -> PSC = Psc ;
	Timer -> ARR = Arr; 
}


/**
	* @brief  Démarre le timer considéré
  * @note   
	* @param  TIM_TypeDef Timer : indique le timer à utiliser par le chronomètre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
void MyTimer_Start(TIM_TypeDef * Timer){
Timer -> CR1 |= TIM_CR1_CEN;
}


/**
	* @brief  Arrêt le timer considéré
  * @note   
	* @param  TIM_TypeDef Timer : indique le timer à utiliser par le chronomètre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
void MyTimer_Stop(TIM_TypeDef * Timer){
Timer -> CR1 &= TIM_CR1_CEN;
}

void (*Pfnc) (void) ; 

/**
	* @brief  Configure le Timer considéré en interruption sur débordement.
  * @note   A ce stade, les interruptions ne sont pas validés (voir  MyTimer_IT_Enable )
	* @param  TIM_TypeDef Timer : indique le timer à utiliser par le chronomètre, TIM1, TIM2, TIM3 ou TIM4
	* 				void (*IT_function) (void) : nom (adresse) de la fonction à lancer sur interruption
	*         int Prio : priorité associée à l'interruption
  * @retval None
  */
void MyTimer_IT_Conf(TIM_TypeDef * Timer, void (*IT_function) (void),int Prio){
	if (Timer == TIM1)	
		NVIC -> IP[TIM1_UP_IRQn] |= Prio<<4;
	else if (Timer == TIM2)
		NVIC -> IP[TIM2_IRQn] |= Prio<<4;	
	
	else if (Timer == TIM3)
		NVIC -> IP[TIM3_IRQn] |= Prio<<4;	
		
	else if (Timer == TIM4)
		NVIC -> IP[TIM4_IRQn] |= Prio<<4;	
	Pfnc = IT_function ; 
}


/**
	* @brief  Autorise les interruptions
  * @note   
	* @param  TIM_TypeDef Timer : indique le timer à utiliser par le chronomètre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
void MyTimer_IT_Enable(TIM_TypeDef * Timer){
	if (Timer == TIM1)	
		NVIC -> ISER[0] |= 1<<TIM1_UP_IRQn;
	else if (Timer == TIM2)
		NVIC -> ISER[0] |= 1<<TIM2_IRQn;
	
	else if (Timer == TIM3)
		NVIC -> ISER[0] |= 1<<TIM3_IRQn;	
		
	else if (Timer == TIM4)
		NVIC -> ISER[0] |= 1<<TIM4_IRQn;
	Timer -> DIER |= TIM_DIER_UIE_Msk ; 
}	


/**
	* @brief  Interdit les interruptions
  * @note   
	* @param  TIM_TypeDef Timer : indique le timer à utiliser par le chronomètre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
void MyTimer_IT_Disable(TIM_TypeDef * Timer){
	if (Timer == TIM1)	
		NVIC -> ISER[0] &= ~ (1<<TIM1_UP_IRQn);
	else if (Timer == TIM2)
		NVIC -> ISER[0] &= ~ (1<<TIM2_IRQn);
	
	else if (Timer == TIM3)
		NVIC -> ISER[0] &=~ (1<<TIM3_IRQn);	
		
	else if (Timer == TIM4)
		NVIC -> ISER[0] &=~ (1<<TIM4_IRQn);
	Timer -> DIER &= ~TIM_DIER_UIE_Msk ; 
}



void TIM1_UP_IRQHandler(void){
	TIM1 -> SR &=~ TIM_SR_UIF_Msk ;
	if (Pfnc != 0)
		(*Pfnc) () ; 
}
void TIM2_IRQHandler(void){
	TIM2 -> SR &=~ TIM_SR_UIF_Msk ;
	if (Pfnc != 0)
		(*Pfnc) () ; 
}
void TIM3_IRQHandler(void){
	TIM3 -> SR &=~ TIM_SR_UIF_Msk ;
	if (Pfnc != 0)
		(*Pfnc) () ; 
}
void TIM4_IRQHandler(void){
	TIM4 -> SR &=~ TIM_SR_UIF_Msk ;
	if (Pfnc != 0)
		(*Pfnc) () ; 
}

