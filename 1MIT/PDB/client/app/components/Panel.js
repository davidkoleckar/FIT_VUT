import React from 'react';
import styled from 'styled-components';

import theme from 'theme';

import { Panel } from 'office-ui-fabric-react/lib/Panel';

const StyledPanel = styled(Panel)`

  &.ms-Panel {
    .ms-Panel-headerText {
      color: ${theme.themePrimary};
    }
    
    .ms-Panel-section {
      font-size: 18px;
      display: block;
      font-weight: 100;
      color: ${theme.themePrimary};
      border-bottom: 1px solid ${theme.themePrimary};
      margin-bottom: 1rem;  
    }
  }

`;

export default StyledPanel;
