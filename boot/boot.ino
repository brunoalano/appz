/**
 * Análise Linguística baseada em Fonemas
 * Projeto de Robótica - TECPUC
 *
 * @author Bruno Alano Medina <bruno@appzlab.com>
 * @license SEE LICENSE
 *
 * Este código é protegido por leis de direito autoral
 * brasileira, lei 9.610/98, onde o intuito é a
 * apresentação a banca avaliadora do TECPUC na
 * matéria de robótica.
 *
 * Trabalho realizado junto com os membros:
 * André Luiz Terencio e Victor Picussa.
 *
 * A Análise Linguística se dá através da detecção de
 * sons projetados pelos fonemas: A (\a\), E(\eeee\),
 * I (\i\), O (\oo\), U (\uh\) e F (\eeffffee\).
 */

/* Macros */
#define DEBUG 0 /* 0 will setup to debug the phonemes. */

/* Project Libraries */
#include <uspeech.h>

/**
 * @namespace appz
 * 
 * Namespace responsável por conter variáveis e classes
 * globais do projeto.
 */
namespace appz
{
  /* Declare the recognizer class */
  signal voice(A0);
}

/**
 * Setup Envirionment
 *
 * Ao executarmos este método, iremos declarar algumas
 * funções de runtime em relação ao Arduino, que não
 * serão JIT (just-in-time).
 */
void setup(void)
{
  /* Initialize the serial comunnication */
  Serial.begin(9600);

  /* Configure the recognizer */
  appz::voice.minVolume = 1500;
  appz::voice.fconstant = 400;
  appz::voice.econstant = 1;
  appz::voice.aconstant = 2;
  appz::voice.vconstant = 3;
  appz::voice.shconstant = 4;
  appz::voice.calibrate();
}

/**
 * Variáveis
 */
uint16_t power;
char phoneme;

/**
 * Loop Runtime
 *
 * Nesta seção, o código irá ser chamado inúmeras vezes
 * para tomar uma série de atitudes.
 *
 * Este é o core do projeto, onde iremos executar o método
 * probabilístico.
 */
void loop(void)
{
  if (DEBUG)
  {
    /* Verificamos se temos uma porta serial aberta para comunicaç~o */
    if (Serial.available() > 0)
    {
      Serial.println("-------------------------------");
      Serial.println("    Plataforma Linguística     ");
      Serial.println("-------------------------------");
      Serial.println("Selecione a opção: \n");
      Serial.println("\t1 - Calibragem de Volume");
      Serial.println("\t2 - Calibragem de Captura de Fonemas");

      /* Lemos o byte em formato de inteiro */
      int chooseOption = Serial.parseInt();

      /* Verificamos se foi selecionada uma opçao */
      switch(chooseOption)
      {
        /**
         * Calibragem do volume do microfone
         */
        case 1:
          /* Aloca o volume do microfone em 2 bytes */
          power = appz::voice.power();

          /* Debug */
          Serial.print("[DEBUG] Voice Power: ");
          Serial.println(power);
          break;

        /**
         * Calibragem de Captura de Fonemas
         */
        case 2:
          /* Alocamos o fonema que foi reproduzido */
          phoneme = appz::voice.getPhoneme();

          /* Verificamos se houve êxito */
          if (phoneme != ' ')
          {
            Serial.print("[Debug] Fonema: ");
            Serial.println(phoneme);
          }
          break;

        /**
         * Opção Padrão
         * Alerta de que esta opção não existe.
         */
        default:
          Serial.println("Inexistente");
     } 
    }
  } /* END DEBUG */
  else
  {
    /**
     * Neste método nós iremos capturar os fonemas a cada
     * repetição do loop do Arduino.
     *
     * Fazendo a análise, iremos montar o modelo probabilístico
     * de cada palavra.
     */
    char phoneme = appz::voice.getPhoneme();

    /*
     * Verificamos se houve a detecção probabilística
     * de algum fonema ou não
     */
    if (phoneme != ' ')
    {
      Serial.print("Fonema detectado: ");
      Serial.println(phoneme);
    }
    else 
    {
      Serial.println("Fonema não detectado");
    }
  }
}
