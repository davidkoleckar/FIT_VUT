<?php
namespace App\Presenters;

use Nette,
    Nette\Forms\Container,
    Nette\Forms\Controls\SubmitButton,
    Nette\Application\UI\Form;



class TerminalPresenter extends BasePresenter {

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
        if (!$this->getUser()->isInRole('admin') and !$this->getUser()->isInRole('spravce')) {
            $this->error('Stránka nebyla nalezena');
        }
    }

    public function renderDefault()
    {
        $this->template->terminaly = $this->database->table('terminal');
        $this->template->gaty = $this->database;
    }

    public function actionRemove($Id_terminal)
    {
        $pole = $this->database->table('gate')->where('ID_terminal', $Id_terminal)->fetchPairs('ID_gate');
        foreach($pole as $item){
            $this->database->table('gate')->where('ID_gate', $item->ID_gate)->delete();
            $this->database->table('dovoluje')->where('ID_gate', $item->ID_gate)->delete();
        }
        $this->database->table('terminal')->where('ID_terminal', $Id_terminal)->delete();
        $this->flashMessage('Terminál byl odstraněn');
        $this->redirect('Terminal:');
    }


    public function MyFormRemoveElementClicked(SubmitButton $button)
    {
        $gates = $button->parent->parent;
        $gates->remove($button->parent, TRUE);
    }

    protected function createComponentAddnewForm()
    {
        $form = new Form;
        $removeEvent = callback($this, 'MyFormRemoveElementClicked');

        $form->addText('nazev', 'Název terminálu: *')
            ->setRequired('Zvolte název terminálu');

        $gates = $form->addDynamic('gates', function (Container $gate) use ($removeEvent) {
            $gate->addText('cislo', 'Číslo gatu:  *')
                ->addRule(Form::INTEGER, 'Číslo gatu musí být číslo')
                ->setDefaultValue('1');

            $post = $this->database->table('typ_letadla')->fetchPairs('ID_typ','typ');

             $gate->addSelect('typ', 'Typ letadla: *', $post)
                ->setPrompt('Zvolte typ letadla na gatu')
                ->setRequired('Zvolte typ letadla na gatu');

            $gate->addSubmit('remove', 'Odebrat gate')
                ->setValidationScope(FALSE) 
                ->onClick[] = [$this, 'MyFormRemoveElementClicked'];
        }, 1);

        $gates->addSubmit('add', 'Přidat další gate')
        ->addCreateOnClick(TRUE);

        $form->addSubmit('send', 'Uložit');
        $form->onSuccess[] = [$this, 'addFormSucceeded'];
        $form->setRenderer(new \App\Model\Bs3FormRenderer);
        return $form;
    }

    public function addFormSucceeded($form, $values)
    {
        $row = $this->database->table('terminal')->insert(array(
            'nazev' => $values->nazev,
        ));

        foreach ($form['gates']->values as $gate) 
        {
            $row2 = $this->database->table('gate')->insert(array(
                'cislo' => $gate['cislo'],
                'ID_terminal' => $row->ID_terminal,
            ));

            $this->database->table('dovoluje')->insert(array(
                'ID_gate' => $row2->ID_gate,
                'ID_typ' => $gate['typ'],
            ));

        }

       
        $this->flashMessage('V pořádku', 'success');
    }


}