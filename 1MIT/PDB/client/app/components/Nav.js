import React from 'react';
import { withRouter } from 'react-router-dom'
import { Nav } from 'office-ui-fabric-react/lib/Nav';
import styled from 'styled-components';

import theme from 'theme';

const StyledNav = styled(Nav).attrs({
    className: "ms-bgColor-black ms-fontColor-white"
})`
    width: 13rem;
    height: 100%;
    padding-top: 1rem;
    	
    .ms-Nav-compositeLink {
        background: transparent;
        margin-bottom: 0.2rem;
        
        &.is-selected {
            .ms-Nav-linkButton {
                background: transparent;
                color: ${theme.white};
            }
        }
        
        .ms-Nav-linkButton {
            color: ${theme.neutralTertiary};
            height: 3rem;
            padding-left: 1.4rem;
            
            &:hover, &:focus {
                color: ${theme.white};
                background-color: ${theme.neutralDark};
            }
            
            .ms-Icon {
                color: ${theme.white};
            }
        }
    }
`;

class AppNav extends React.Component {
    render() {
        const { history, match } = this.props;

        const redirect = (e, element) => {
            e.preventDefault();
            history.push(element.url);
        };

        return (
            <StyledNav
                onLinkClick={redirect}
                selectedKey={location.pathname.indexOf('vozidla') > -1 ? 'vozidla' : undefined}
                groups={ [{
                    links: [
                        { name: 'Přehled', key: 'prehled', url: '/', icon: 'AutoRacing' },
                        { name: 'Areal', key: 'areal', url: '/areal', icon: 'Taskboard' },
                        { name: 'Vozidla', key: 'vozidla', url: '/vozidla', icon: 'Car' },
                        { name: 'Kategorie', key: 'kategorie', url: '/kategorie', icon: 'PlanView' },
                        { name: 'Uživatelé', key: 'uzivatele', url: '/uzivatele', icon: 'People' },
                    ]
                }] }
            />
        );
    }
}


export default withRouter(AppNav);
