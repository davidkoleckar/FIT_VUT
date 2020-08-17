<?php
namespace App\Presenters;

use Nette,
    Nette\Forms\Container,
    Nette\Forms\Controls\SubmitButton,
    Nette\Application\UI\Form;



class LetenkaPresenter extends BasePresenter {

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
        $this->template->letenky = $this->database->table('letenka');
        $this->template->db = $this->database;
    }

    public function actionRemove($ID_letenka)
    {
        $this->database->table('letenka')->where('ID_letenka', $ID_letenka)->delete();
        $this->flashMessage('Letenka byla odstraněna');
        $this->redirect('Letenka:');
    }


    protected function createComponentAddnewForm()
    {
        $form = new Form;
       
        $form->addText('cena', 'Cena letenky (kč): *')
            ->addRule(Form::INTEGER, 'Cena musí být číslo')
            ->setRequired('Zvolte cenu');

        $trida = [
            'první' => 'První',
            'turistická' => 'Turistická',
            'business' => 'Business',
        ];

        $form->addSelect('trida', 'Třída: *', $trida)
            ->setPrompt('Zvolte třídu')
            ->setRequired('Zvolte třídu');

        $umisteni = [
            'uprostřed' => 'uprostřed',
            'okýnko' => 'okýnko',
            'ulička' => 'ulička',
        ];

        $form->addSelect('umisteni', 'Umístění v letadle: *', $umisteni)
            ->setPrompt('Zvolte umístění')
            ->setRequired('Zvolte umístění');

        $post = $this->database->table('let')->fetchPairs('ID_let','kam');

        $form->addSelect('let', 'Let: *', $post)
            ->setPrompt('Zvolte let')
            ->setRequired('Zvolte let');

        $form->addText('pocet', 'Počet letenek: *')
            ->addRule(Form::INTEGER, 'Musí být číslo')
            ->setDefaultValue('1')
            ->setRequired('Zvolte počet');

        $form->addSubmit('send', 'Uložit');
        $form->onSuccess[] = [$this, 'addFormSucceeded'];
        $form->setRenderer(new \App\Model\Bs3FormRenderer);
        return $form;
    }

    public function addFormSucceeded($form, $values)
    {
        for($i=0;$i<$values->pocet;$i++)
        {
            $this->database->table('letenka')->insert(array(
                'cena' => $values->cena,
                'trida' => $values->trida,
                'umisteni' => $values->umisteni,
                'ID_let' => $values->let,
            ));
        }
       
        $this->flashMessage('V pořádku', 'success');
        $this->redirect('Letenka:');
    }


}