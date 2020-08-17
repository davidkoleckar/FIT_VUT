import React from 'react';

import { CommandBar } from 'office-ui-fabric-react/lib/CommandBar';
import styled from 'styled-components';

const StyledCommandBar = styled(CommandBar)`

    &.ms-CommandBar {
        margin: 0;
        padding: 0.4rem 2rem;
        height: auto;
        
        .ms-CommandBar-sideCommands {
            padding-right: 0;
        }
    }

`;

export default StyledCommandBar;
