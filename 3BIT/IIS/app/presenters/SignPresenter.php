<?php

namespace App\Presenters;

use Nette;
use Nette\Application\UI\Form;
use Nette\Security\Passwords;


class SignPresenter extends Nette\Application\UI\Presenter
{

    protected function createComponentSignInForm()
    {
        $form = new Form;
        $form->addText('username', 'Uživatelské jméno: *')
            ->setRequired('Prosím vyplňte své uživatelské jméno.');

        $form->addPassword('password', 'Heslo: *')
            ->setRequired('Prosím vyplňte své heslo.');

        $form->addCheckbox('remember', 'Zapamatovat přihlášení');

        $form->addSubmit('send', 'Přihlásit');

        $form->onSuccess[] = $this->signInFormSubmitted;
        $form->setRenderer(new \App\Model\Bs3FormRenderer);
        return $form;
    }

    public function actionOut()
    {
        $this->getUser()->logout();
        $this->flashMessage('Odhlášení bylo úspěšné.');
        $this->redirect('Homepage:');
    }

    public function signInFormSubmitted($form)
    {

        $values = $form->getValues();

        if ($values->remember) {
            $this->getUser()->setExpiration('+ 7 days', FALSE);
        } else {
            $this->getUser()->setExpiration('+ 20 minutes', TRUE);
        }

        try {
            $this->getUser()->login($values->username, $values->password);
        } catch (Nette\Security\AuthenticationException $e) {
            $form->addError($e->getMessage());
            return;
        }

        $this->redirect('Homepage:');
    }

}