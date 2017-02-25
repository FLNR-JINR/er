Распады
=======

Пакет ExpertRoot обладает собственным подходом добавления распадов в процесс транспорта частиц. Этот 
подход основан на на возможности пользователя написать свою собственную обработку Stepping Action
библиотеки транспорта и возможности добавлять в стэк треков новые треки.

У такого подхода есть два достоинства:
  1. Исследователь может заложить только ту информацию о распаде, которой обладает.
  2. Распад встроен в процесс транспорта. Поэтому нет необходимости заранее наигрывать параметры
  первичного иона в момент распада.
 
Класс распада
-------------
Реализация своего распада добавляется с помощью создания класса, унаследованного от `ERDecay`. Коды 
распадов располодены в дириктории `decays`.

В новом классе необходимо написать реализации методов: `Init`, `BeginEvent` , `FinishEvent` , `Stepping` .

В методе `Init` необходимо проверить все ли ионы и частицы, которые учавствуют в распаде добавлены 
в объект распада и в Root библиотеку частиц - `TDatabasePDG`.

::

  if (fInputIon) {
    fInputIonPDG = TDatabasePDG::Instance()->GetParticle(fInputIonName);
    if ( ! fInputIonPDG ) {
        std::cerr  << "ERTextDecay: Ion " << fInputIonName << " not found in database!"<< endl;
        return kFALSE;
    }
  }
  else{
    std::cerr  << "Input ion not defined"<< endl;
    return kFALSE;
  }
  
Так же необходимо прверить все ли входные текстовые файлы и другие настройки распада указаны.

::

  if (fFileName == ""){
    cerr << "File for " << fName << " decay not defined!" << endl;
    return kFALSE;
  }

Метод `BeginEvent` вызывается в начале каждого события. Стандартно в этом методе необходимо реинициализировать
часть переменных распада и разыграть вероятностные характеристики распада, в частности, позицию распада.

::

  fDecayFinish = kFALSE;
  if (fUniform){
    fDecayPosZ = fRnd->Uniform(fUniformA, fUniformB);
  }
  if (fExponential){
    fDecayPosZ = fExponentialStart + fRnd->Exp(fExponentialTau);
  }

Метод `Stepping` вызывается на каждом шаге транспорта всех частиц. В нём закладывается условие распада 
и добавляются новые треки в стек треков. Типичная структура этого метода представлена далее.

Проверка того, что мы рассматриваем трекинг интересующего нас иона и установка малого шага трека для 
того чтобы максимально точно выбрать позицию распада:

::

  if(!fDecayFinish && gMC->TrackPid() == fInputIonPDG->PdgCode()){
    gMC->SetMaxStep(0.01);
    gMC->TrackPosition(fDecayPos);

Провека что распад выполниться именно на этом шаге:

::

    if (fDecayPos.Z() > fDecayPosZ){
      gMC->TrackMomentum(fInputIonV);
      //Add new ion
      Int_t newTrackNb;
      vector<TLorentzVector> decay = fDecays[gMC->CurrentEvent()];

Добавление новых частиц в стек для трнспорта:
     
::

      gMC->GetStack()->PushTrack(1,gMC->GetStack()->GetCurrentTrackNumber(), fOutputIonPDG->PdgCode(),
                           outputIonV.Px(),outputIonV.Py(),outputIonV.Pz(),
                           outputIonV.E(), fDecayPos.X(), fDecayPos.Y(), fDecayPos.Z(),
                           gMC->TrackTime(), 0., 0., 0.,
                           kPDecay, newTrackNb, fOutputIonPDG->Mass(), 0);
                           
Окончание распада. Остановка первичного иона. Возвращение максимального шага транспорта. Сохранение
характеристик распада в объект `MCEventHeader` .
 
      fDecayFinish = kTRUE;
      gMC->StopTrack();
      gMC->SetMaxStep(10000.);
      SaveToEventHeader();
    }
  }
  return kTRUE;

