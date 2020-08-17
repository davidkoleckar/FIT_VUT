import React from 'react';

import { PanelType } from 'office-ui-fabric-react/lib/Panel';
import { Dropdown, DropdownMenuItemType } from 'office-ui-fabric-react/lib/Dropdown';
import { PrimaryButton, DefaultButton } from 'office-ui-fabric-react/lib/Button';
import { MessageBar, MessageBarType } from 'office-ui-fabric-react/lib/MessageBar';
import { TagPicker } from 'office-ui-fabric-react/lib/components/pickers/TagPicker/TagPicker';

import Panel from 'components/Panel';
import TextField from 'components/TextField';
import CheckboxGroup from 'components/CheckboxGroup';
import Slider from 'components/Slider';

let _testTags = [
  'black',
  'blue',
  'brown',
  'cyan',
  'green',
  'magenta',
  'mauve',
  'orange',
  'pink',
  'purple',
  'red',
  'rose',
  'violet',
  'white',
  'yellow'
].map(item => ({ key: item, name: item }));

class AddPanel extends React.PureComponent { // eslint-disable-line react/prefer-stateless-function

  constructor() {
    super();
    this._onRenderFooterContent = this._onRenderFooterContent.bind(this);
    this._onFilterChanged = this._onFilterChanged.bind(this);

    this.state = {
      checked: [],
    }
  }

  _getTextFromItem(item) {
    return item.name;
  }

  _onFilterChanged(filterText, tagList) {
    return filterText ? _testTags.filter(tag => tag.name.toLowerCase().indexOf(filterText.toLowerCase()) === 0).filter(item => !this._listContainsDocument(item, tagList)) : [];
  }

  _listContainsDocument(tag, tagList) {

    if (!tagList || !tagList.length || tagList.length === 0) {
      return false;
    }
    return tagList.filter(compareTag => compareTag.key === tag.key).length > 0;
  }

  render() {
     return (
          <Panel
              isOpen={ false }
              type={ PanelType.medium }
              onDismiss={ this.props.onClosePanel }
              headerText="header"
              closeButtonAriaLabel='Close'
              onRenderFooterContent={ this._onRenderFooterContent }
          >
            <div className="row mb-3">
              <div className="col-8">
                <MessageBar>Info - lorem ipsum dolor sit amet, a elit sem interdum consectetur adipiscing elit.</MessageBar>
              </div>
            </div>

            <span className="ms-Panel-section">Udaje auta</span>

            <div className="row mb-3">
                <div className="col-3">Nazev</div>
                <div className="col">
                  <TextField
                    placeholder='Budova E15'
                    ariaLabel='Please enter text here'
                  />
                </div>
            </div>

            <div className="row mb-3">
                <div className="col-3">Podlahova plocha</div>
                <div className="col">
                  <TextField
                    addonAfter
                    addonString={<span>m<sup>3</sup></span>}
                    placeholder='125'
                    ariaLabel='Please enter text here'
                  /></div>
              </div>

            <div className="row mb-3">
              <div className="col-3">Provozní doba</div>
              <div className="col">
                <div className="row no-gutters align-items-center">
                  <div className="col">
                    <Slider
                      min={ 1 }
                      max={ 24 }
                      step={ 1 }
                      showValue={ true }
                      onChange={ (value) => console.log(value) }
                    />
                  </div>
                  <div className="col-auto">
                    hodin
                  </div>
                </div>
                </div>
              </div>

              <div className="row mb-3">
                <div className="col-3">Otevřeno v</div>
                <div className="col">
                  <Dropdown
                    placeHolder='Vyberte'
                    multiSelect
                    options={
                      [
                        { key: 'header_1', text: 'Dny v týdnu', itemType: DropdownMenuItemType.Header },
                        { key: '1', text: 'Pondělí', },
                        { key: '2', text: 'Úterý' },
                        { key: '3', text: 'Středa' },
                        { key: '4', text: 'Čtvrtek' },
                        { key: '5', text: 'Pátek' },
                        { key: 'divider_1', text: '-', itemType: DropdownMenuItemType.Divider },
                        { key: '6', text: 'Sobota' },
                        { key: '7', text: 'Neděle' },
                      ]
                    }
                  />
                </div>
              </div>
          </Panel>
        );
    }

    _onRenderFooterContent() {
        return (
            <div>
                <PrimaryButton
                    onClick={ this.props.onClosePanel }
                    className="mr-2"
                >
                    Přidat
                </PrimaryButton>
                <DefaultButton
                    onClick={ this.props.onClosePanel }
                >
                    Zrušit
                </DefaultButton>
            </div>
        );
    }
}

export default AddPanel;
