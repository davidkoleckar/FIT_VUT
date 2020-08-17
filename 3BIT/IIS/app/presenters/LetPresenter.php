<?php
namespace App\Presenters;

use Nette,
    Nette\Forms\Container,
    Nette\Forms\Controls\SubmitButton,
    Nette\Application\UI\Form;



class LetPresenter extends BasePresenter {

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
        $this->template->lety = $this->database->table('let');
        $this->template->db = $this->database;
    }

    public function renderaddclen($Id_let)
    {

    }

    public function actionRemove($ID_let)
    {
        $this->database->table('let')->where('ID_let', $ID_let)->delete();
        $this->flashMessage('Let byl odstraněn');
        $this->redirect('Let:');
    }


     protected function createComponentAddnewForm()
    {
        $form = new Form;
        $form->addText('datumod','Datum odletu: *')
            ->setType('date')
            ->setDefaultValue('dd.mm.rrrr')
            ->setRequired('zadejte datum odletu');

        $form->addText('casod','Čas odletu: *')
            ->setType('time')
            ->setDefaultValue('hh.mm')
            ->setRequired('zadejte čas odletu');

        $form->addText('datumdo','Předpokládaný datum příletu: *')
            ->setType('date')
            ->setDefaultValue('dd.mm.rrrr')
            ->setRequired('zadejte datum příletu');

        $form->addText('casdo','Předpokládaný čas příletu: *')
            ->setType('time')
            ->setDefaultValue('hh.mm')
            ->setRequired('zadejte předpokládaný čas příletu');

        $form->addText('kam', 'Cílová destinace (název města): *')
            ->setRequired('Zvolte cílovou destinaci');

        $form->addText('max', 'Maximální počet cestujících:')
            ->setRequired('Zvolte maximální počet cestujících')
            ->addRule(Form::RANGE, 'Počet musí být od 1 do 800', [1, 800])
            ->setType('number');

        $post = $this->database->table('terminal')->fetchPairs('ID_terminal','nazev');

        $form->addSelect('terminal', 'Na kterém terminálu: *', $post)
            ->setPrompt('Zvolte terminál')
            ->setRequired('Zvolte terminál');

        $form->addDependentSelectBox('name', 'Vyberte gate: *', array($form["terminal"]), function ($values) {
            $data =  new \NasExt\Forms\Controls\DependentSelectBoxData();
            $post2 = $this->database->table('gate')->where('ID_terminal',$values['terminal'])->fetchPairs('ID_gate','cislo');
            return $data->setItems($post2);
   
        })->setPrompt('Vyberte gate');


        $form->addSubmit('send', 'Uložit');
        $form->onSuccess[] = [$this, 'addFormSucceeded'];
        $form->setRenderer(new \App\Model\Bs3FormRenderer);
        return $form;
    }

    public function addFormSucceeded($form, $values)
    {
        $datum_odletu = $values->datumod . " " . $values->casod . ":00";
        $datum_priletu = $values->datumdo . " " . $values->casdo . ":00";
        $pom = $this->database->table('dovoluje')->where('ID_gate',$values->name)->fetch();
        $letadlo = $this->database->table('letadlo')->where('ID_typ',$pom->ID_typ)->fetch();
        $this->database->table('let')->insert(array(
            'kam' => $values->kam,
            'cas_odletu'=> $datum_odletu,
            'cas_priletu' => $datum_priletu,
            'max_cestujicich' => $values->max,
            'ID_letadlo' => $letadlo->ID_letadlo,
            'ID_terminal' => $values->terminal,
            'ID_gate' => $values->name,
        ));
       
        $this->flashMessage('V pořádku', 'success');
        $this->redirect('Let:');
    }



    protected function createComponentAddclenForm()
    {
        $form = new Form;

        $Id = $this->getParameter('Id_let');

        $clen = $this->database->table('clen')->fetchPairs('ID_clen','jmeno');

        $default = $this->database->table('ma')->where('ID_let', $Id)->fetchPairs('ID_ma','ID_clen');
        $form->addCheckboxList('list', 'Posádka: *', $clen)
            ->setDefaultValue($default);


        $form->addSubmit('send', 'Uložit');
        $form->onSuccess[] = [$this, 'addclenFormSucceeded'];
        $form->setRenderer(new \App\Model\Bs3FormRenderer);
        return $form;
    }

    public function addclenFormSucceeded($form, $values)
    {
        $Id = $this->getParameter('Id_let');
        
        $default = $this->database->table('ma')->where('ID_let', $Id)->fetchPairs('ID_ma','ID_clen');
        if(!empty($default))
        {
            foreach($default as $item)
            {
                $this->database->table('ma')->where('ID_let', $Id)->delete();
            }
            foreach($values->list as $pom)
            {
                $this->database->table('ma')->insert(array(
                    'ID_clen' => $pom,
                    'ID_let' => $Id,
                ));
            }
        }
        else
        {
            foreach($values->list as $pom)
            {
                $this->database->table('ma')->insert(array(
                    'ID_clen' => $pom,
                    'ID_let' => $Id,
                ));
            }
        }
        $this->flashMessage('V pořádku', 'success');
        $this->redirect('Let:');
    }


}