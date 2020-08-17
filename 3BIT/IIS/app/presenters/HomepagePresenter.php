<?php

namespace App\Presenters;

use Nette;
use Nette\Application\UI\Form;
use App\Model;
use Nette\Templates\Template;


class HomepagePresenter extends BasePresenter
{
  /** @var Nette\Database\Context */
  private $database;
  public  $t ;
  public function __construct(Nette\Database\Context $database)
  {
      $this->database = $database;
  }


  protected function createComponentAddnewForm()
  {
      $form = new Form;
      $form->addText('datumod','Datum odletu: *')
        ->setType('date')
        ->setDefaultValue('dd.mm.rrrr')
        ->setRequired('zadejte datum odletu');

      /*$trida = [
            'první' => 'První',
            'turistická' => 'Turistická',
            'business' => 'Business',
        ];

      $form->addSelect('trida', 'Třída: *', $trida)
          ->setPrompt('Zvolte třídu')
          ->setRequired('Zvolte třídu');*/

      $post = $this->database->table('let')->fetchPairs('kam','kam');

      $form->addSelect('let', 'Cílová destinace: *', $post)
          ->setPrompt('Zvolte destinaci')
          ->setRequired('Zvolte destinaci');
      

      $form->addSubmit('send', 'Vyhledat');
      $form->onSuccess[] = callback($this, 'addFormSucceeded');
      $form->setRenderer(new \App\Model\Bs3FormRenderer);
      return $form;
  }

  public function addFormSucceeded($form, $values)
  {

     $this->redirect('Homepage:vysledek',(array)$values);

  }

  public function renderDefault()
  {
  }

  public function renderVysledek($values)
  {
    $trida = $this->getParameter('trida');
    $datum = $this->getParameter('datumod');
    $kam = $this->getParameter('let');
    $datum = $datum." 00:00:00";
    $post2 = $this->database->table('let')->where('cas_odletu > ? AND kam = ? ', $datum, $kam);
    $this->template->kam = $kam;
    $this->template->test=$post2;
    $this->template->db = $this->database;

  }

}
