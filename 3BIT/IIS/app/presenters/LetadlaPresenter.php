<?php
namespace App\Presenters;

use Nette,
    Nette\DateTime,
    Nette\Application\UI\Form;



class LetadlaPresenter extends BasePresenter {

    private $database;

    public function __construct(Nette\Database\Context $database)
    {
        $this->database = $database;
    }

    protected function startup()
    {
        parent::startup();
        if (!$this->getUser()->isLoggedIn()) {
            $this->redirect('Sign:in');
        }
        if (!$this->getUser()->isInRole('admin') and !$this->getUser()->isInRole('technik')) {
            $this->error('Stránka nebyla nalezena');
        }
    }


    public function renderDefault()
    {
        $this->template->letadla = $this->database->table('letadlo');
    }

    public function actionRemove($Id_letadlo)
    {
        $this->database->table('letadlo')->where('ID_letadlo', $Id_letadlo)->delete();
        $this->flashMessage('Letadlo bylo odstraněno');
        $this->redirect('Letadla:');
    }

    public function actionRevize($Id_letadlo)
    {
        $h = $this->database->table('letadlo')->where('ID_letadlo', $Id_letadlo);
        $d = Date("Y.m.j", Time());
        $h->update(array(
            'datum_posledni_revize' =>$d
        ));
        $this->flashMessage('Revize byla zaznamenána.');
        $this->redirect('Letadla:');
    }
   
    protected function createComponentAddnewForm()
    {
        $form = new Form;
        $form->addText('typ', 'Typ: *')
            ->setRequired();

        $form->addText('vyrobce', 'Výrobce: *')
            ->setRequired();

        $form->addText("date", "Datum výroby: *")
            ->setRequired("Datum výroby je povinný údaj!")
            ->setAttribute("class", "dtpicker")
            ->setAttribute("placeholder", "rrrr.mm.dd")
            ->addRule($form::PATTERN, "Datum musí být ve formátu rrrr.mm.dd", "((19|20)\d\d)\.(0[1-9]|1[012])\.(0[1-9]|[12][0-9]|3[01])");
 
        $post = $this->database->table('users')->where('role', 'technik')->fetchPairs('id','username');


        $form->addSelect('technik', 'Technik: *', $post)
            ->setPrompt('Zvolte technika')
            ->setRequired('Zvolte technika');

        $form->addSubmit('send', 'Uložit');
        $form->onSuccess[] = [$this, 'addFormSucceeded'];
        $form->setRenderer(new \App\Model\Bs3FormRenderer);
        return $form;
    }

    public function addFormSucceeded($form, $values)
    {
        $row = $this->database->table('typ_letadla')->insert(array(
            'vyrobce' => $values->vyrobce,
            'typ' => $values->typ,
        ));

        $this->database->table('letadlo')->insert(array(
            'datum_vyroby' => $values->date,
            'ID_technik' => $values->technik,
            'ID_typ'=> $row->ID_typ,
        ));

        $this->flashMessage('V pořádku', 'success');
        $this->redirect('Letadla:');
    }

}